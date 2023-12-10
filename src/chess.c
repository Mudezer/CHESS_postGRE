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

#include "utils/builtins.h"
#include "libpq/pqformat.h"
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

static ChessBoard * chessboard_make(const char * fen)
{
    elog(LOG, "chessboard_make\n");
    ChessBoard *c = palloc0(sizeof(ChessBoard)); 
    SCL_boardFromFEN(c->board, fen);
    strcpy(c->fen, fen);

    return c;
}

/*****************************************************************************
 * Parser & verification of errors
 *****************************************************************************/

bool isBoardLegal(const char * fen){
    bool boardIsLegal = true;
    char a[100],b[2],c[20],d[20];
    int e,f;
    char g[2];
    boardIsLegal = sscanf(fen, "%s %s %s %s %d %d", a, b, c, d, &e, &f) == 6;//Vérifie que le FEN n’est pas trop court et respecte le format requis
    int counter = 0;
    char* tok;
    tok = strtok(a, "/");
    while(tok != NULL) {
      tok = strtok(NULL, "/");
      counter++;
    }
    if (counter != MAX_MOVE_LENGTH + 1) boardIsLegal = false; //Vérifie que le board soit bien 8 chaines de caractères séparé par 7 "/"
    if(sscanf(fen, "%s %s %s %s %d %d %s", a, b, c, d, &e, &f,g) == 7) boardIsLegal = false; //Vérifie que le FEN ne soit pas trop lon
    return boardIsLegal;
}

/**
 * Parse an input that has a FEN format to create a ChessBoard struct
 * @param fen : the FEN format input
 * @return a pointer to the ChessBoard struct
*/
static ChessBoard * chessboard_parse(const char fen[MAX_FEN_LENGTH])
{
  if (!isBoardLegal(fen)) ereport(ERROR,(errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),errmsg("invalid input syntax for a chess board")));
  else return chessboard_make(fen);
}

bool isGameLegal(const char * pgn){
  char gamePGN[MAX_PGN_LENGTH];
  strcpy(gamePGN,pgn);
  bool gameIsLegal = true;
  int counter = 0;
  int incr = 0;
  char* tok = strtok(gamePGN, " .");
  if(!(strcmp(tok,"1")||strcmp(tok,"1-0")||strcmp(tok,"0-1"))) gameIsLegal = false;
  tok = strtok(NULL, " .");
  while(tok != NULL) {
      if(incr == 2){
        int tokInt = atoi(tok);
        if(tokInt != counter/2+1){ //teste que le numéro matche bien le nombre du move 
          if(!(strcmp(tok,"1/2-1/2")||strcmp(tok,"1-0")||strcmp(tok,"0-1"))) gameIsLegal = false; //teste que  tok ="1-0" ou "0-1" ou égalité
        } 
        incr = 0;
      } else {
        if(strlen(tok)>8) gameIsLegal = false;
        counter++;
        incr++;
      }
    tok = strtok(NULL, " .");
  }
  return gameIsLegal;
}

/**
 * Parse an input that has a PGN format to create a ChessGame struct
 * @param pgn : the PGN format input
 * @return a pointer to the ChessGame struct
*/
static ChessGame * chessgame_parse(const char * pgn)
{
 if (!isGameLegal(pgn)) ereport(ERROR,(errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),errmsg("invalid input syntax for a chess game")));
  else return chessgame_make(pgn);
}

/*****************************************************************************
 * Functions to implement
 *****************************************************************************/

/**
 * @param chessgame the game to analyze
 * @param n the number of moves to print
 * @returns ChessGame truncated to the n first moves
*/
static ChessGame * getFirstMoves(ChessGame * chessgame, int n)
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

/**
 * @param chessgame the game to analyze
 * @param chessboard the board to find
 * @param moveNumber the length of the interval to check
 * @returns bool true if the game has the board, false otherwise
*/
static bool hasBoard(ChessGame* chessgame, ChessBoard * chessboard , int moveNumber)
{
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
    char* firstMoves = getFirstMoves(chessgame, moveNumber)->pgn;
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

    pfree(board_tofind); 
    pfree(board); 
    pfree(record);

    return isSame;
}


/*****************************************************************************
 * C-SQL Linking Functions
 *****************************************************************************/

/**
 * The following functions are the link between the SQL and the C code
*/

/*********************************IN/OUT**************************************/

PG_FUNCTION_INFO_V1(chessboard_in);
Datum chessboard_in(PG_FUNCTION_ARGS)
{
  char *fen = PG_GETARG_CSTRING(0); 
  PG_RETURN_ChessBoard_P(chessboard_parse(fen)); 

}

PG_FUNCTION_INFO_V1(chessboard_out);
Datum chessboard_out(PG_FUNCTION_ARGS) 
{
  ChessBoard *c = PG_GETARG_ChessBoard_P(0); 
  PG_RETURN_CSTRING(c->fen);
}

PG_FUNCTION_INFO_V1(chessgame_in);
Datum chessgame_in(PG_FUNCTION_ARGS)
{
  char *pgn = PG_GETARG_CSTRING(0); 
  PG_RETURN_ChessGame_P(chessgame_parse(pgn)); 
}

PG_FUNCTION_INFO_V1(chessgame_out);
Datum chessgame_out(PG_FUNCTION_ARGS)
{
  ChessGame *c = PG_GETARG_ChessGame_P(0); 
  PG_RETURN_CSTRING(c->pgn);
}


PG_FUNCTION_INFO_V1(getFirstMoves2);
Datum getFirstMoves2(PG_FUNCTION_ARGS)
{
  ChessGame *cgame = PG_GETARG_ChessGame_P(0);
  int32 n = PG_GETARG_INT32(1);
  ChessGame *result = getFirstMoves(cgame, n);
  PG_RETURN_ChessGame_P(result);
}

PG_FUNCTION_INFO_V1(getBoard2);
Datum getBoard2(PG_FUNCTION_ARGS)
{
  ChessGame *cgame = PG_GETARG_ChessGame_P(0);
  int32 n = PG_GETARG_INT32(1);
  ChessBoard *result = getBoard(cgame, n);
  PG_RETURN_ChessBoard_P(result);
}

PG_FUNCTION_INFO_V1(hasOpening2);
Datum hasOpening2(PG_FUNCTION_ARGS)
{
  ChessGame *cgame = PG_GETARG_ChessGame_P(0);
  ChessGame *dgame = PG_GETARG_ChessGame_P(1);
  bool result = hasOpening(cgame, dgame);
  PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(hasBoard2);
Datum hasBoard2(PG_FUNCTION_ARGS)
{
  ChessGame *cgame = PG_GETARG_ChessGame_P(0);
  ChessBoard * cboard = PG_GETARG_ChessBoard_P(1);
  int32 n = PG_GETARG_INT32(2);
  bool result = hasBoard(cgame, cboard, n);
  PG_RETURN_BOOL(result);
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

PG_FUNCTION_INFO_V1(chessboard_contains);
Datum chessboard_contains(PG_FUNCTION_ARGS)
{
  ChessGame *value = chessgame_make(PG_GETARG_POINTER(0));
  ChessBoard *template = chessboard_make(PG_GETARG_POINTER(1));
  int *moveNumber = palloc0(sizeof(int));
  *moveNumber = SCL_recordLength(value->record)+1;

  bool result = hasBoard(value, template , *moveNumber);
  PG_RETURN_BOOL(result);

}

PG_FUNCTION_INFO_V1(chessboard_contained); //  right arg is contained by left arg
Datum chessboard_contained(PG_FUNCTION_ARGS)
{
  ChessBoard *value = PG_GETARG_ChessBoard_P(0);
  ChessGame *query = PG_GETARG_ChessGame_P(1);
  int moveNumber = SCL_recordLength(query->record)+1;

  bool result = hasBoard(query, value , moveNumber);
  PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(chessboard_overlap);
Datum chessboard_overlap(PG_FUNCTION_ARGS)
{
  ChessGame *value = PG_GETARG_ChessGame_P(0);
  ChessGame *query = PG_GETARG_ChessGame_P(1);

  int na = SCL_recordLength(value->record);
  int nb = SCL_recordLength(query->record);

  bool result = false;

  for(int i = 0; i <= na ; i++){
    for(int j = 0; j <= nb ; j++){
      if(query->record[i] == value->record[j]){
        result = true;
        break;
      }
    }
  }
  result = false;
  PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(chessboard_eq);
Datum chessboard_eq(PG_FUNCTION_ARGS)
{
  ChessBoard *value = PG_GETARG_ChessBoard_P(0);
  ChessBoard *query = PG_GETARG_ChessBoard_P(1);

  PG_RETURN_BOOL(strcmp(value->fen, query->fen) == 0);
}

/*****************************************************************************
 * Gin Support Functions
*****************************************************************************/

PG_FUNCTION_INFO_V1(chessboard_cmp);
Datum chessboard_cmp(PG_FUNCTION_ARGS)
{
  ChessBoard *value = PG_GETARG_ChessBoard_P(0);
  ChessBoard *query = PG_GETARG_ChessBoard_P(1);

  PG_RETURN_INT32(strcmp(value->fen, query->fen));
}

PG_FUNCTION_INFO_V1(chessboard_extractValue);
Datum chessboard_extractValue(PG_FUNCTION_ARGS)
{
  ChessGame *a = chessgame_make(PG_GETARG_POINTER(0));

  //number of keys (number of boards in the game)
  int32 *nkeys = (int32*) PG_GETARG_POINTER(1); 

  //length of the game +1 (for the initial board)
  int len = SCL_recordLength(a->record)+1; 
  *nkeys = (int32) len;
  
  //array of boards in Datum format (convenenient for postgres)
  Datum *entries = palloc0(sizeof(Datum) * len); 
  entries[0] = PointerGetDatum(chessboard_make("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR"));
  SCL_Board *tempBoard = palloc0(sizeof(SCL_Board));
  char tempFen[SCL_FEN_MAX_LENGTH];

  for(int i =1; i <= len; i++){
    SCL_recordApply(a->record, *tempBoard, i);
    SCL_boardToFEN((char *) tempBoard, tempFen);
    char *tempState = strtok(tempFen, " ");
    ChessBoard *temp = chessboard_make(tempState);
    entries[i] = PointerGetDatum(temp);
  }
  pfree(tempBoard);
  PG_RETURN_POINTER(entries);
}


PG_FUNCTION_INFO_V1(chessboard_extractQuery);
Datum chessboard_extractQuery(PG_FUNCTION_ARGS)
{
  int32 *nentries = (int32 *) PG_GETARG_POINTER(1);
	uint16 strategy = PG_GETARG_UINT16(2);
	Datum	   *entries;

  if(strategy == 1){ // right arg(query) overlaps left arg(index)
    entries = (Datum *) DatumGetPointer(DirectFunctionCall2(chessboard_extractValue, PG_GETARG_DATUM(0), PointerGetDatum(nentries)));
    if(entries == NULL){
      elog(ERROR, "entries is NULL | strategy 3\n");
    }
  }
  else if(strategy ==2){ // left arg(index) contains right arg(query)
    ChessBoard *a = PG_GETARG_ChessBoard_P(0);
    char *tempBoard = a->fen;
    char *tempState = strtok(tempBoard, " ");
    ChessBoard *query = chessboard_make(tempState);
    entries = palloc0(sizeof(Datum) * *nentries);
    entries[0] = PointerGetDatum(query);
    *nentries = 1;
  }
  else if(strategy == 3){ // left arg(index) is contained by right arg(query)
    entries = (Datum *) DatumGetPointer(DirectFunctionCall2(chessboard_extractValue, PG_GETARG_DATUM(0), PointerGetDatum(nentries)));
    if(entries == NULL){
      elog(LOG, "entries is NULL | strategy 3\n");
    }
  }
  else if(strategy == 4){ // left arg(index) equals right arg(query)
    ChessBoard *a = PG_GETARG_ChessBoard_P(0);
    char *tempBoard = a->fen;
    char *tempState = strtok(tempBoard, " ");
    ChessBoard *query = chessboard_make(tempState);
    entries = palloc0(sizeof(Datum) * *nentries);
    entries[0] = PointerGetDatum(query);
    *nentries = 1;
  }
  else{
    elog(LOG, "Unknown strategy number: %d", strategy);
    entries = NULL;
  }

  PG_RETURN_POINTER(entries);
}

PG_FUNCTION_INFO_V1(chessboard_consistent);
Datum chessboard_consistent(PG_FUNCTION_ARGS)
{
  bool *check = (bool *) PG_GETARG_POINTER(0);
  uint16 strategy = PG_GETARG_UINT16(1);

  int32 nkeys = PG_GETARG_INT32(3);

  bool *recheck = (bool *) PG_GETARG_POINTER(5);
  bool res = false;
  int32 i;

  switch(strategy)
  {
    case 1: //au moins un des element dans check est vrai donc res = true
      *recheck = false;
      res = true;
      break;
    case 2:
      /* result is not lossy */
			*recheck = false;
			/* Must have all elements in check[] true */
			res = true;
			for (i = 0; i < nkeys; i++)
			{
				if (!check[i])
				{
					res = false;
					break;
				}
			}
			break;
    case 3:
      *recheck = true;
      res = true;
      break;
    case 4:
      /* we will need recheck */
			*recheck = true;
			/* Must have all elements in check[] true */
			res = true;
			for (i = 0; i < nkeys; i++)
			{
				if (!check[i])
				{
					res = false;
					break;
				}
			}
			break;
    default:
      elog(LOG, "chessboard_consistent: unknown strategy number: %d", strategy);
  }
  PG_RETURN_BOOL(res);
}

PG_FUNCTION_INFO_V1(getBoards);
Datum getBoards(PG_FUNCTION_ARGS)
{
  ChessGame *a = PG_GETARG_ChessGame_P(0);
  int n = PG_GETARG_INT32(1);
  SCL_Board *tempBoard = palloc0(sizeof(SCL_Board));

  //array of boards in Datum format (convenenient for postgres)
  Datum *entries = palloc0(sizeof(Datum) * n+1); 
  char tempFen[SCL_FEN_MAX_LENGTH];
  entries[0] = PointerGetDatum(chessboard_make("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR"));

  for(int i = 1; i <= n; i++){
    SCL_recordApply(a->record, *tempBoard, i);
    SCL_boardToFEN((char *) tempBoard, tempFen);
    char *tempState = (char*) strtok(tempFen, " ");
    ChessBoard *temp = chessboard_make(tempState);
    entries[i] = PointerGetDatum(temp);
  }
  pfree(tempBoard);
  PG_RETURN_POINTER(entries);
}