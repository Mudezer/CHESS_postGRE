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

#include "utils/builtins.h"
#include "libpq/pqformat.h"

#include "chess.h"

PG_MODULE_MAGIC;

/*****************************************************************************/

/*****************************************************************************
 * Make
 *****************************************************************************/

static ChessGame * chessgame_make(const char * pgn)
{
    ChessGame *c = malloc(sizeof(ChessGame)); // TODO replace by palloc 
    strcpy(c->pgn, pgn);
    SCL_recordFromPGN(c->record, pgn);
    return c;
}


static ChessBoard * chessboard_make(const char fen[MAX_FEN_LENGTH])
{
    ChessBoard *c = malloc(sizeof(ChessBoard)); // TODO replace by palloc 
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
 * Cast functions
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
  ChessBoard *c = PG_GETARG_ChessBoard_P(0); // Pointer to the SQL datatype & cast it in struct C
  char *fen = chessboard_to_str(c); // To only have the FEN 
  PG_FREE_IF_COPY(c, 0);
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
  ChessGame *c = PG_GETARG_ChessGame_P(0); // Pointer to the SQL datatype & cast it in struct C
  char *pgn = chessgame_to_str(c); // To only have the PGN 
  PG_FREE_IF_COPY(c, 0);
  PG_RETURN_CSTRING(pgn);
}



/************************************CAST*************************************/

PG_FUNCTION_INFO_V1(chessboard_cast_from_text);
Datum chessboard_cast_from_text(PG_FUNCTION_ARGS)
{
  text *txt = PG_GETARG_TEXT_P(0);
  char *fen = DatumGetCString(DirectFunctionCall1(textout, PointerGetDatum(txt)));
  PG_RETURN_ChessBoard_P(chessboard_parse(fen));
}


PG_FUNCTION_INFO_V1(chessboard_cast_to_text);
Datum chessboard_cast_to_text(PG_FUNCTION_ARGS)
{
  ChessBoard *c  = PG_GETARG_ChessBoard_P(0);
  text *out = (text *)DirectFunctionCall1(textin, PointerGetDatum(chessboard_to_str(c)));
  PG_FREE_IF_COPY(c, 0);
  PG_RETURN_TEXT_P(out);
}



PG_FUNCTION_INFO_V1(chessgame_cast_from_text);
Datum chessgame_cast_from_text(PG_FUNCTION_ARGS)
{
  text *txt = PG_GETARG_TEXT_P(0);
  char *pgn = DatumGetCString(DirectFunctionCall1(textout, PointerGetDatum(txt)));
  PG_RETURN_ChessGame_P(chessgame_parse(pgn));
}


PG_FUNCTION_INFO_V1(chessgame_cast_to_text);
Datum chessgame_cast_to_text(PG_FUNCTION_ARGS)
{
  ChessGame *c  = PG_GETARG_ChessGame_P(0);
  text *out = (text *)DirectFunctionCall1(textin, PointerGetDatum(chessgame_to_str(c)));
  PG_FREE_IF_COPY(c, 0);
  PG_RETURN_TEXT_P(out);
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
    char *openingPGN = malloc(sizeof(char)*MAX_PGN_LENGTH);
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
  ChessGame *cgame = PG_GETARG_ChessGame_P(0);
  int32 n = PG_GETARG_INT32(1);
  ChessGame *result = getFirstMoves(cgame, n);
  PG_FREE_IF_COPY(cgame, 0);
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
  ChessGame *cgame = PG_GETARG_ChessGame_P(0);
  ChessGame *dgame = PG_GETARG_ChessGame_P(1);
  bool result = hasOpening(cgame, dgame);
  PG_FREE_IF_COPY(cgame, 0);
  PG_FREE_IF_COPY(dgame, 1);
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
    SCL_Board *board_tofind = malloc(sizeof(SCL_Board));
    SCL_boardFromFEN(*board_tofind, board_FEN);
    char board_tofindStr[65]; // Increase the size by 1 to accommodate the null terminator
    strncpy(board_tofindStr, *board_tofind, 64);
    board_tofindStr[64] = '\0'; // Add the null terminator

    // Boards of the game
    char* firstMoves = getFirstMoves(gamePGN, moveNumber);
    SCL_Record *record = malloc(sizeof(SCL_Record));
    SCL_recordFromPGN(*record, firstMoves);
    SCL_Board *board = malloc(sizeof(SCL_Board));
    char boardStr[65];
    while(counter < moveNumber)
    {
        counter++;
        SCL_recordApply(*record, *board, counter);
        strncpy(boardStr, *board, 64);
        boardStr[64] = '\0';
        isSame = (strcmp(boardStr, board_tofindStr) == 0);
    }

    free(board_tofind); 
    free(board); 
    free(record);

    return isSame;
}


PG_FUNCTION_INFO_V1(hasBoard2);
Datum hasBoard2(PG_FUNCTION_ARGS)
{
  ChessGame *cgame = PG_GETARG_ChessGame_P(0);
  ChessBoard * cboard = PG_GETARG_ChessBoard_P(1);
  int32 n = PG_GETARG_INT32(2);
  bool result = hasBoard(cgame, cboard, n);
  PG_FREE_IF_COPY(cgame, 0);
  PG_FREE_IF_COPY(cboard, 1);
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
    char* boardFEN = malloc(sizeof(char)*MAX_FEN_LENGTH);

    SCL_Record *record = malloc(SCL_RECORD_MAX_SIZE);
    SCL_recordFromPGN(*record, gamePGN);
    SCL_Board *board = malloc(SCL_RECORD_MAX_SIZE);
    SCL_recordApply(*record, *board, moveNumber);
    SCL_boardToFEN(*board, boardFEN);

    free(record);

    return chessboard_make(boardFEN);
}


PG_FUNCTION_INFO_V1(getBoard2);
Datum getBoard2(PG_FUNCTION_ARGS)
{
  ChessGame *cgame = PG_GETARG_ChessGame_P(0);
  int32 n = PG_GETARG_INT32(1);
  ChessBoard *result = getBoard(cgame, n);
  PG_FREE_IF_COPY(cgame, 0);
  PG_RETURN_ChessBoard_P(result);
}

