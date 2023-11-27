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

#include "smallchesslib.h"
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
