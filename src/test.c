#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

#define BOARD_SIZE 8
#define BOARD_SIZE_SQUARE 64
#define MAX_FEN_LENGTH 100


typedef struct{
    char *castling; // roque
    int fullmove_number; // each 2 halfmoves clock
} postgres_chessboard;



static postgres_chessboard * postgres_chessboard_make(char *castling, int fullmove_number)
{
  postgres_chessboard *c = malloc(sizeof(postgres_chessboard));
  c->castling = strdup(castling);
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
    char str[] = "KQkq 1"; 
    postgres_chessboard c;
    fen_parse(str);
    return 0;
}