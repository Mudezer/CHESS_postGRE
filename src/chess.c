#include "postgres.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include 



#define BOARD_SIZE 8
#define BOARD_SIZE_SQUARE 64
#define MAX_FEN_LENGTH 100



typedef struct{

    char fen[MAX_FEN_LENGTH]; //
    char board[BOARD_SIZE][BOARD_SIZE]; // état de la table
    char turn; // à qui de jouer
    char castling[4]; // roque
    char en_passant[2]; // en passant
    char halfmove_clock; // halfmove clock
    char fullmove_number; // each 2 halfmoves clock

} postgres_chessboard;


