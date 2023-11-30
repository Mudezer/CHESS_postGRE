#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include "chess.h"

#define BOARD_SIZE 8
#define BOARD_SIZE_SQUARE 64
#define MAX_FEN_LENGTH 100



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
 * Main 
 *****************************************************************************/

int main()
{
    /* Interesting functions
    ChessBoard   ->     - SCL_Board
                        - SCL_BoardToFEN
                        - SCL_BoardFromFEN
                        SCL_BoardMakeMove

    ChessMove   ~>  - SCL_Record
                    - SCL_recordFromPGN

    ChessGame  ->   SCL_Game
                    SCL_gameInit
                    SCL_gameMakeMove
    */

    char * pgn1 = "1.f3 e5 2. g4 Qh4 0-1";
    char * pgn2 = "1.f3 e5";
    char * str0 = "rnbqkbnr/pp1p1ppp/8/2p5/2BpP3/8/PPP2PPP/RNBQK1NR w KQkq c6 0 4"; 

    
    ChessBoard * CB1 = chessboard_parse(str0);
        printf("'%s'\n", CB1->board);
        printf("'%s'\n", CB1->fen);
        printf("'%s'\n", chessboard_to_str(CB1));

    ChessGame * CG1 = chessgame_parse(pgn2);
        printf("'%s'\n", CG1->pgn);
        // test record
        SCL_Board board0;
        SCL_boardInit(board0);
        printf("'%s'\n", board0);
        SCL_recordApply(CG1->record, board0, 2);
        printf("'%s'\n", board0);
        printf("'%s'\n", chessgame_to_str(CG1));



    SCL_Board board1; 
    SCL_Board board2; 
    SCL_Record r1;
    SCL_Record r2;

    // HasOpening() not the good type for input and result & getBoard() almost done
    SCL_recordFromPGN(r1, pgn1);
    SCL_recordFromPGN(r2, pgn2);
    SCL_recordApply(r1, board1, 2); // The first n half-moves applied to the board -> getBoard() ALMOST DONE
    SCL_recordApply(r2, board2, 2);   
    uint8_t a = SCL_boardsDiffer(board1, board2);
        //printf("'%u'\n", a); // 0 is true & 1 false

    return 0;
}