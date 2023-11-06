/*#include "postgres.h"*/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

/*#include <smallchesslib.h>
#include "utils/builtins.h"
#include "libpq/pqformat.h"
*/

PG_MODULE_MAGIC;

#define BOARD_SIZE 8
#define BOARD_SIZE_SQUARE 64
#define MAX_FEN_LENGTH 100

/*****************************************************************************/

/* Structure to represent the chessgame */
/*
typedef struct
{
    string     halfMove,
    int        halfMoveNumber;
} ChessHalfMove;

typedef struct
{
    string      pgn,
    ChessHalfMove[] halfMoves;

} ChessGame;
*/


typedef struct{
    char *fen; // TODO fen[MAX_FEN_LENGTH];
    char *turn; // turn à qui de jouer
    char *castling; // castling[4] roque
    char *en_passant; // en_passant[2]
    int halfmove_clock; // halfmove clock
    int fullmove_number; // each 2 halfmoves clock
} postgres_chessboard2;


/*****************************************************************************/

/*****************************************************************************/

/*****************************************************************************/

static postgres_chessboard2 * postgres_chessboard_make2(char *fen, char *turn, char *castling, char *en_passant, int halfmove_clock, int fullmove_number)
{
  postgres_chessboard2 *c = malloc(sizeof(postgres_chessboard2));
  c->fen = strdup(fen);
  c->turn = strdup(turn);
  c->castling = strdup(castling);
  c->en_passant = strdup(en_passant);
  c->halfmove_clock = halfmove_clock;
  c->fullmove_number = fullmove_number;
        
  return c;
}

/*****************************************************************************/


static postgres_chessboard2 * fen_parse2(char * str)
{

    char *fen; //
    char *turn; // à qui de jouer*/
    char *castling; // roque
    char *en_passant; // en passant
    int halfmove_clock; // halfmove clock*/
    int fullmove_number; // each 2 halfmoves clock

    char* rest[14] = {0}; // 11 char pointers  
    size_t n = 0; 

    /*Parser for elements*/    
    char *ptr = strtok(str, " ");    

    rest[n++] = ptr; 
    printf("'%s'\n", ptr);
    fen = ptr;

    ptr = strtok(NULL, " ");
    rest[n++] = ptr; 
    printf("'%s'\n", ptr);
    turn = ptr;
    
    ptr = strtok(NULL, " ");
    rest[n++] = ptr; 
    printf("'%s'\n", ptr);
    castling = ptr;

    ptr = strtok(NULL, " ");
    rest[n++] = ptr; 
    printf("'%s'\n", ptr);
    en_passant = ptr;

    ptr = strtok(NULL, " "); 
    rest[n++] = ptr; 
    printf("'%s'\n", ptr);
    halfmove_clock = atoi(ptr);

    ptr = strtok(NULL, " "); 
    rest[n++] = ptr; 
    printf("'%s'\n", ptr);
    fullmove_number = atoi(ptr);

    return postgres_chessboard_make2(fen, turn, castling, en_passant, halfmove_clock, fullmove_number);

}





static char * chessboard_to_str(const postgres_chessboard2 *c)
{
  char *str = malloc (sizeof (char) *100);
  sprintf(str, "%s %s %s %s %d %d", c->fen,c->turn, c->castling, c->en_passant, c->halfmove_clock, c->fullmove_number);

  return str;
}


/*****************************************************************************/

/*****************************************************************************/

/*****************************************************************************/


int main()
{
    char str[] = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    postgres_chessboard2 *c;
    c = fen_parse2(str);
        /*printf("'%s'\n", c->fen);
        printf("'%s'\n", c->turn);
        printf("'%s'\n", c->castling);
        printf("'%s'\n", c->en_passant);
        printf("'%d'\n", c->halfmove_clock);
        printf("'%d'\n", c->fullmove_number);*/
   
    char *str2;
    str2 = chessboard_to_str(c);
    printf("'%s'\n", str2);
    return 0;
}
