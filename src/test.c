#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include "smallchesslib.h"

#define BOARD_SIZE 8
#define BOARD_SIZE_SQUARE 64
#define MAX_FEN_LENGTH 100


typedef struct{
    char *castling[4]; // roque
    int fullmove_number; // each 2 halfmoves clock
} postgres_chessboard;



static postgres_chessboard * postgres_chessboard_make(char *castling, int fullmove_number)
{
  postgres_chessboard *c = malloc(sizeof(postgres_chessboard));
  strcpy(c->castling, castling);
  c->fullmove_number = fullmove_number;
    printf("'%s'\n", c->castling);
    printf("'%d'\n", c->fullmove_number);
  return c;
}

static postgres_chessboard * fen_parse(char * str)
{

    char *castling[4]; // roque
    int fullmove_number;

    char* rest[12] = {0}; // 11 char pointers  
    size_t n = 0; 

    /*Parser for elements*/    
    char *ptr = strtok(str, " ");    

    rest[n++] = ptr; 
    printf("'%s'\n", ptr);
    *castling = ptr;
        printf("'%s'\n", *castling);

    ptr = strtok(NULL, " "); 
    rest[n++] = ptr; 
    printf("'%s'\n", ptr);
    fullmove_number = atoi(ptr);
        printf("'%d'\n", fullmove_number);
 
    return postgres_chessboard_make(*castling, fullmove_number);

}





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
    char * str0 = "rnbqkbnr/pppp1ppp/8/4p3/4P3/8/PPPP1PPP/RNBQKBNR w KQkq e6 0 2"; 
    //postgres_chessboard c;
    //fen_parse(str);
    SCL_Board board0;
    SCL_Board board1; 
    SCL_Board board2; 
    //SCL_boardInit(board1); // The initial state
        //printf("'%s'\n", board);

    SCL_boardFromFEN(board0, str0);
        //printf("'%s'\n", board0);

    //uint8_t squareFrom = SCL_stringToMove(*moveString, uint8_t *resultFrom, uint8_t *resultTo, char *resultPromotion);
    //char *SCL_moveToString(SCL_Board board, uint8_t s0, uint8_t s1, char promotion, char *string);
    //SCL_boardMakeMove(board, uint8_t squareFrom, uint8_t squareTo, char promotePiece);
    
    SCL_Record r1;
    SCL_Record r2;

    /*SCL_recordFromPGN(r, pgn); // The entire game is in the record
        //printf("'%s'\n", r); // Not printable
        printf("'%s'\n", pgn); // FEN print
        printf("'%x'\n", r); // Not valid
        printf("'%x'\n", *r); // Not valid
        printf("'%u'\n", r); // Not valid
        printf("'%u'\n", *r); // Not valid
    */



    // HasOpening() not the good type for input and result & getBoard() almost done
    SCL_recordFromPGN(r1, pgn1);
    SCL_recordFromPGN(r2, pgn2);
    SCL_recordApply(r1, board1, 2); // The first n half-moves applied to the board -> getBoard() ALMOST DONE
    SCL_recordApply(r2, board2, 2);   
    uint8_t a = SCL_boardsDiffer(board1, board2);
        printf("'%u'\n", a); // 0 is true & 1 false

    /*char * str[MAX_FEN_LENGTH];
    SCL_boardToFEN(board, str); // To see the modification
        printf("'%s'\n", board); // Board state without the others infos
        printf("'%s'\n", str); // FEN print
    */
    
    return 0;
}