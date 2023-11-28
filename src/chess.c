/*
 * chess.c
 */

#include <postgres.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
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
    c->pgn = pgn;
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

static char * chessboard_to_str(const ChessBoard *c)
{
  return c->fen;
}


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



/*********************************CONTRUCTOR**********************************/


/*
PG_FUNCTION_INFO_V1(chessboard_constructor);
Datum chessboard_constructor(PG_FUNCTION_ARGS)
{
  char * fen = PG_GETARG_CSTRING(0);
  PG_RETURN_ChessBoard_P(chessboard_make(fen));
}



PG_FUNCTION_INFO_V1(chessgame_constructor);
Datum chessgame_constructor(PG_FUNCTION_ARGS)
{
  char * pgn = PG_GETARG_CSTRING(0);
  PG_RETURN_ChessGame_P(chessgame_make(pgn));
}
*/



/*****************************************************************************
 * Functions
 *****************************************************************************/


/*****************************************************************************/
