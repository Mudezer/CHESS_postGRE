/*
 * chess.c
 */

#include <postgres.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <float.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include "utils/elog.h"
#include "utils/memutils.h"
// #include "postgres/src/backend/access/gin/ginarrayproc.c"

#include "utils/builtins.h"
#include "libpq/pqformat.h"
// #include "utils/elog.h" // see if needed
#include "chess.h"

PG_MODULE_MAGIC;

/*****************************************************************************
 * Make
 *****************************************************************************/

static ChessGame * chessgame_make(const char * pgn)
{
    ChessGame *c = palloc0(sizeof(ChessGame));
    strcpy(c->pgn, pgn);
    SCL_recordFromPGN(c->record, pgn);
    return c;
}

static ChessBoard * chessboard_make(const char fen[MAX_FEN_LENGTH])
{
    ChessBoard *c = palloc0(sizeof(ChessBoard)); 
    SCL_boardFromFEN(c->board, fen);
    strcpy(c->fen, fen);
    return c;
}

/*****************************************************************************
 * Parser & verification of errors TODO
 *****************************************************************************/

/**
 * Parse an input that has a FEN format to create a ChessBoard struct
 * @param fen : the FEN format input
 * @return a pointer to the ChessBoard struct
*/
static ChessBoard * chessboard_parse(const char fen[MAX_FEN_LENGTH])
{
  /* // Test if the make is well done ?
  char * a, b, c, d, e, f;
  if (sscanf(fen, "%s %s %s %s %s %s", &a, &b, &c, &d, &e, &f) != 6)
    ereport(ERROR,(errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
      errmsg("invalid input syntax for type %s: \"%s\"", "complex", str)));
    */
  return chessboard_make(fen);
}


/**
 * Parse an input that has a PGN format to create a ChessGame struct
 * @param pgn : the PGN format input
 * @return a pointer to the ChessGame struct
*/
static ChessGame * chessgame_parse(const char * pgn)
{
  /* // Test if the make is well done ?
  char * a;
  if (sscanf(fen, "%s", &a) != 1)
    ereport(ERROR,(errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
      errmsg("invalid input syntax for type %s: \"%s\"", "complex", str)));
    */
  return chessgame_make(pgn);
}


/*****************************************************************************
 * GETTERS
 *****************************************************************************/

/**
 * Getter of the FEN format of a ChessBoard
 * @param fen : the ChessBoard struct
 * @return the FEN format of the ChessBoard
*/
static char * chessboard_to_str(const ChessBoard *c)
{
  return c->fen;
}

/**
 * Getter of the PGN format of a ChessGame
 * @param fen : the ChessGame struct
 * @return the PGN format of the ChessGame
*/
static char * chessgame_to_str(const ChessGame *c)
{
  return c->pgn;
}

/*****************************************************************************
 * C-SQL Linking Functions
 *****************************************************************************/

/**
 * The following functions are the link between the SQL and the C code
*/

/*********************************IN/OUT**************************************/

PG_FUNCTION_INFO_V1(chessboard_in); // Declare the function
Datum chessboard_in(PG_FUNCTION_ARGS)
{
  char *fen = PG_GETARG_CSTRING(0); // String input from the SQL
  PG_RETURN_ChessBoard_P(chessboard_parse(fen)); // Send the pointer to the C struct
  // à noter que le coté SQL s'en fout du type de pointeur qu'on renvoie du moment qu'on lui indique que c'est un pointeur
}

PG_FUNCTION_INFO_V1(chessboard_out);
Datum chessboard_out(PG_FUNCTION_ARGS) // Datum to declare a generic data type
{
  ChessBoard *c = chessboard_make(PG_GETARG_ChessBoard_P(0)); // Pointer to the SQL datatype & cast it in struct C
  char *fen = chessboard_to_str(c); // To only have the FEN 
  //PG_FREE_IF_COPY(c, 0);
  PG_RETURN_CSTRING(fen);
}

PG_FUNCTION_INFO_V1(chessgame_in); // Declare the function
Datum chessgame_in(PG_FUNCTION_ARGS)
{
  char *pgn = PG_GETARG_CSTRING(0); // String input from the SQL
  PG_RETURN_ChessGame_P(chessgame_parse(pgn)); // Send the pointer to the C struct
  // à noter que le coté SQL s'en fout du type de pointeur qu'on renvoie du moment qu'on lui indique que c'est un pointeur
}

PG_FUNCTION_INFO_V1(chessgame_out);
Datum chessgame_out(PG_FUNCTION_ARGS) // Datum to declare a generic data type
{
  ChessGame *c = chessgame_make(PG_GETARG_ChessGame_P(0)); // Pointer to the SQL datatype & cast it in struct C
  char *pgn = chessgame_to_str(c); // To only have the PGN 
  //PG_FREE_IF_COPY(c, 0);
  PG_RETURN_CSTRING(pgn);
}

/*****************************************************************************
 * Functions to implement
 *****************************************************************************/

/**
 * @param chessgame the game to analyze
 * @param n the number of moves to print
 * @returns ChessGame truncated to the n first moves
*/
static ChessGame * getFirstMoves(ChessGame * chessgame, int n) //TODO: case no space after point
{
    char * gamePGN = chessgame->pgn;
    char gamePGN_copy[MAX_PGN_LENGTH]; strcpy(gamePGN_copy, gamePGN);
    char *openingPGN = palloc0(sizeof(char)*MAX_PGN_LENGTH);
    int counter = 0;
    int incr = 0;
    char* tok = strtok(gamePGN_copy, " ");
    strcat(openingPGN, tok);
    tok = strtok(NULL, " ");
    while((counter < n) && (tok != NULL)) {
        strcat(openingPGN, " ");
        strcat(openingPGN, tok);
        if(incr == 2)
            incr = 0;
        else {
            counter++;
            incr++;
            }
        
        tok = strtok(NULL, " ");
    }
    return chessgame_make(openingPGN);
}


PG_FUNCTION_INFO_V1(getFirstMoves2);
Datum getFirstMoves2(PG_FUNCTION_ARGS)
{
  ChessGame *cgame = chessgame_make(PG_GETARG_ChessGame_P(0));
  int32 n = PG_GETARG_INT32(1);
  ChessGame *result = getFirstMoves(cgame, n);
  //PG_FREE_IF_COPY(cgame, 0);
  PG_RETURN_ChessGame_P(result);
}


/**
 * @param chessgame the game to analyze
 * @param opening the opening to find
 * @returns bool true if the game has the opening, false otherwise
*/
static bool hasOpening(ChessGame* chessgame, ChessGame* openingGame)
{
    char * gamePGN = chessgame->pgn;
    char * openingPGN = openingGame->pgn;
    bool isSame = (strstr(gamePGN, openingPGN) != NULL);
    return isSame;
}


PG_FUNCTION_INFO_V1(hasOpening2);
Datum hasOpening2(PG_FUNCTION_ARGS)
{
  ChessGame *cgame = chessgame_make(PG_GETARG_ChessGame_P(0));
  ChessGame *dgame = chessgame_make(PG_GETARG_ChessGame_P(1));
  bool result = hasOpening(cgame, dgame);
  //PG_FREE_IF_COPY(cgame, 0);
  //PG_FREE_IF_COPY(dgame, 1);
  PG_RETURN_BOOL(result);
}


/**
 * @param chessgame the game to analyze
 * @param chessboard the board to find
 * @param moveNumber the length of the interval to check
 * @returns bool true if the game has the board, false otherwise
*/
static bool hasBoard(ChessGame* chessgame, ChessBoard * chessboard , int moveNumber)
{
    char * gamePGN = chessgame->pgn;
    char* board_FEN = chessboard->fen;
    int counter = 0;
    bool isSame = false;

    // Board to find
    SCL_Board *board_tofind = palloc0(sizeof(SCL_Board));
    SCL_boardFromFEN(*board_tofind, board_FEN);
    char board_tofindStr[65]; // Increase the size by 1 to accommodate the null terminator
    strncpy(board_tofindStr, *board_tofind, 64);
    board_tofindStr[64] = '\0'; // Add the null terminator

    // Boards of the game
    char* firstMoves = getFirstMoves(gamePGN, moveNumber);
    SCL_Record *record = palloc0(sizeof(SCL_Record));
    SCL_recordFromPGN(*record, firstMoves);
    SCL_Board *board = palloc0(sizeof(SCL_Board));
    char boardStr[65];
    while((counter <= moveNumber) && (isSame == false))
    {
        SCL_recordApply(*record, *board, counter);
        strncpy(boardStr, *board, 64);
        boardStr[64] = '\0';
        isSame = (strcmp(boardStr, board_tofindStr) == 0);
        counter++;
    }

    //free(board_tofind); 
    //free(board); 
    //free(record);

    return isSame;
}


PG_FUNCTION_INFO_V1(hasBoard2);
Datum hasBoard2(PG_FUNCTION_ARGS)
{
  ChessGame *cgame = chessgame_make(PG_GETARG_ChessGame_P(0));
  ChessBoard * cboard = chessboard_make(PG_GETARG_ChessBoard_P(1));
  int32 n = PG_GETARG_INT32(2);
  bool result = hasBoard(cgame, cboard, n);
  //PG_FREE_IF_COPY(cgame, 0);
  //PG_FREE_IF_COPY(cboard, 1);
  PG_RETURN_BOOL(result);
}


/**
 * Getter of the board at a specific halfmove
 * @param chessgame the entire game
 * @param moveNumber the number of the first halfmoves
 * @returns ChessBoard state at the specific halfmove
*/
static ChessBoard * getBoard(ChessGame* chessgame, int moveNumber)
{
    char * gamePGN = chessgame->pgn;
    char* boardFEN = palloc0(sizeof(char)*MAX_FEN_LENGTH);

    SCL_Record *record = palloc0(SCL_RECORD_MAX_SIZE);
    SCL_recordFromPGN(*record, gamePGN);
    SCL_Board *board = palloc0(SCL_RECORD_MAX_SIZE);
    SCL_recordApply(*record, *board, moveNumber);
    SCL_boardToFEN(*board, boardFEN);

    //free(record);

    return chessboard_make(boardFEN);
}


PG_FUNCTION_INFO_V1(getBoard2);
Datum getBoard2(PG_FUNCTION_ARGS)
{
  ChessGame *cgame = chessgame_make(PG_GETARG_ChessGame_P(0));
  int32 n = PG_GETARG_INT32(1);
  ChessBoard *result = getBoard(cgame, n);
  //PG_FREE_IF_COPY(cgame, 0);
  PG_RETURN_ChessBoard_P(result);
}



/*****************************************************************************
 * B-Tree Operators
 *****************************************************************************/

PG_FUNCTION_INFO_V1(chessgame_eq);
Datum chessgame_eq(PG_FUNCTION_ARGS)
{
  char *a = DatumGetPointer(PG_GETARG_DATUM(0));
  char *b = DatumGetPointer(PG_GETARG_DATUM(1));
  PG_RETURN_BOOL(strcmp(a, b) == 0);
}

PG_FUNCTION_INFO_V1(chessgame_le);
Datum chessgame_le(PG_FUNCTION_ARGS)
{
  char *a = DatumGetPointer(PG_GETARG_DATUM(0));
  char *b = DatumGetPointer(PG_GETARG_DATUM(1));
  PG_RETURN_BOOL(strcmp(a, b) <= 0);
}

PG_FUNCTION_INFO_V1(chessgame_lt);
Datum chessgame_lt(PG_FUNCTION_ARGS)
{
  char *a = DatumGetPointer(PG_GETARG_DATUM(0));
  char *b = DatumGetPointer(PG_GETARG_DATUM(1));
  PG_RETURN_BOOL(strcmp(a, b) < 0);
}

PG_FUNCTION_INFO_V1(chessgame_ge);
Datum chessgame_ge(PG_FUNCTION_ARGS)
{
  char *a = DatumGetPointer(PG_GETARG_DATUM(0));
  char *b = DatumGetPointer(PG_GETARG_DATUM(1));
  PG_RETURN_BOOL(strcmp(a, b) >= 0);
}

PG_FUNCTION_INFO_V1(chessgame_gt);
Datum chessgame_gt(PG_FUNCTION_ARGS)
{
  char *a = DatumGetPointer(PG_GETARG_DATUM(0));
  char *b = DatumGetPointer(PG_GETARG_DATUM(1));
  PG_RETURN_BOOL(strcmp(a, b) > 0);
}

/*****************************************************************************
 * B-Tree Support Function
*****************************************************************************/

PG_FUNCTION_INFO_V1(chessgame_cmp);
Datum chessgame_cmp(PG_FUNCTION_ARGS)
{
  char *a = DatumGetPointer(PG_GETARG_DATUM(0));
  char *b = DatumGetPointer(PG_GETARG_DATUM(1));
  PG_RETURN_INT32(strcmp(a, b));
}


/*****************************************************************************
 * GIN Operators
*****************************************************************************/

PG_FUNCTION_INFO_V1(chessboard_overlap);
Datum chessboard_overlap(PG_FUNCTION_ARGS)
{
  elog(LOG, "chessboard_overlap\n");
  ChessGame *a = 
}


PG_FUNCTION_INFO_V1(chessboard_eq);
Datum chessboard_eq(PG_FUNCTION_ARGS)
{
  elog(LOG, "chessboard_eq\n");
  ChessBoard *a = DatumGetPointer(PG_GETARG_DATUM(0));
  ChessBoard *b = DatumGetPointer(PG_GETARG_DATUM(1));
  PG_RETURN_BOOL(strcmp(a, b) == 0);
}

/*****************************************************************************
 * GIN INDEX SUPPORT FUNCTIONS
*****************************************************************************/

PG_FUNCTION_INFO_V1(chessboard_cmp);
Datum chessboard_cmp(PG_FUNCTION_ARGS)
{
  elog(LOG, "chessboard_cmp\n");
  ChessBoard *a = DatumGetPointer(PG_GETARG_DATUM(0));
  ChessBoard *b = DatumGetPointer(PG_GETARG_DATUM(1));
  PG_RETURN_INT32(strcmp(a, b));
}


PG_FUNCTION_INFO_V1(chessboard_extractValue);
Datum chessboard_extractValue(PG_FUNCTION_ARGS)
{
  elog(LOG, "chessboard_extractValue\n");
  ChessGame *a = PG_GETARG_ChessGame_P(0); //inputValue (data from dB)
  int32 *nkeys = (int32*) PG_GETARG_POINTER(1); //number of keys (number of boards in the game)
  bool **nullFlags = (bool**) PG_GETARG_POINTER(2); //null flags
  int len = SCL_recordLength(a->record)+1; //length of the game +1 (for the initial board)
  *nkeys = (int32) len; //number of boards in the game


  Datum *entries = palloc0(sizeof(Datum) * *nkeys); //array of boards in Datum format (convenenient for postgres)
  // je sais pas si il faut mettre ça "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" ou pas => faut essayer pour savoir
  entries[0] = PointerGetDatum(chessboard_make("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR")); //initial board
  
  SCL_Board *tempBoard = palloc0(sizeof(SCL_Board));
  char tempFen[SCL_FEN_MAX_LENGTH];


  for(int i =1; i <= len; i++){
    SCL_recordApply(a->record, *tempBoard, i);
    SCL_boardToFEN(tempBoard, tmp);
    char *tempState = strtok(tmp, " ");
    ChessBoard *temp = chessboard_make(tempState)
    entries[i] = PointerGetDatum(temp);
  }

  free(tempBoard);
  PG_RETURN_POINTER(entries);

}

// Datum *extractQuery(Datum query, int32 *nkeys, StrategyNumber n, bool **pmatch, Pointer **extra_data, bool **nullFlags, int32 *searchMode)

PG_FUNCTION_INFO_V1(chessboard_extractQuery);
Datum chessboard_extractQuery(PG_FUNCTION_ARGS)
{
  elog(LOG, "chessboard_extractQuery\n");
  int32 *nentries = (int32 *) PG_GETARG_POINTER(1);
	StrategyNumber strategy = PG_GETARG_UINT16(2);
	int32	   *searchMode = (int32 *) PG_GETARG_POINTER(6);
	Datum	   *entries;

   // number of entries in the query, normally it should be only 1 chessboard
  ChessBoard *temp = palloc0(sizeof(ChessBoard));

  if(strategy == 1){ // right arg(query) overlaps left arg(index)
    ChessBoard *a = PG_GETARG_ChessBoard_P(0);
    char *tempBoard = chessboard_to_str(a);
    char *tempState = strtok(tempBoard, " ");
    ChessBoard *query = chessboard_make(tempState);
    entries = palloc0(sizeof(Datum) * *nentries);
    entries[0] = PointerGetDatum(query);
    *nentries = (int32) 1;
  }
  else if(stategy ==2){ // left arg(index) contains right arg(query)
    ChessBoard *a = PG_GETARG_ChessBoard_P(0);
    char *tempBoard = chessboard_to_str(a);
    char *tempState = strtok(tempBoard, " ");
    ChessBoard *query = chessboard_make(tempState);
    entries = palloc0(sizeof(Datum) * *nentries);
    entries[0] = PointerGetDatum(query);
    *nentries = (int32) 1;
  }
  else if(strategy == 3){ // left arg(index) is contained by right arg(query)
    ChessGame *a = PG_GETARG_ChessGame_P(0);
    entries = chessboard_extractValue(a, &nentries, void);
    entries = (Datum *)
			DatumGetPointer(DirectFunctionCall2(chessboard_extractValue,
												PG_GETARG_ChessGame_P(0),
												PointerGetDatum(nentries)))
    if(entries == NULL){
      elog()
    }
  }
  else if(strategy == 4){ // left arg(index) equals right arg(query)
    ChessBoard *a = PG_GETARG_ChessBoard_P(0);
    char *tempBoard = chessboard_to_str(a);
    char *tempState = strtok(tempBoard, " ");
    ChessBoard *query = chessboard_make(tempState);
    entries = palloc0(sizeof(Datum) * *nentries);
    entries[0] = PointerGetDatum(query);
    *nentries = (int32) 1;
  }
  else{
    elog(ERROR, "Unknown strategy number: %d", strategy);
    entries = NULL;
  }

  PG_RETURN_POINTER(entries);
}

// PG_FUNCTION_INFO_V1(chessboard_consistent);
// Datum chessboard_consistent(PG_FUNCTION_ARGS)
// {
//   elog(LOG, "chessboard_consistent\n");
//   StrategyNumber strategy = PG_GETARG_UINT16(2);
//   bool *recheck = (bool *) PG_GETARG_POINTER(5);
//   *recheck = false;
//   PG_RETURN_BOOL(true);
// }

/*****************************************************************************
 * UTILS
*****************************************************************************/

// /**
//  * @param chessgame the game to analyze
//  * @returns boards an array of the boards of the game in FEN truncated format
// */
// static char ** chessgame_generate_boards(ChessGame *a)
// {

//   int len = SCL_recordLength(a->record);

//   char **boards = palloc0(sizeof(char*)*len+1);
//   boards[0] = palloc0(sizeof(char)*SCL_FEN_MAX_LENGTH);
//   strcpy(boards[0],"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");

//   SCL_Board *temp = palloc0(sizeof(SCL_Board));

//   for(int i = 0; i < len; i++){

//     SCL_recordApply(a->record, *temp, i+1);
//     boards[i+1] = palloc0(sizeof(char)*SCL_FEN_MAX_LENGTH);
//     char tmp[SCL_FEN_MAX_LENGTH];
//     SCL_boardToFEN(temp, tmp);
//     char* tmp2 = strtok(tmp, " ");
//     strcpy(boards[i+1], tmp2);
//   } 

//   free(temp);

//   return boards;
// }
