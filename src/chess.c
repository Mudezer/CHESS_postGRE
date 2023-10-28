#include "postgres.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


#define BOARD_SIZE 8
#define BOARD_SIZE_SQUARE 64
#define MAX_FEN_LENGTH 100

/*****************************************************************************/

/* Structure to represent a half move */
typedef struct
{
    char*       halfMove;
    int         halfMoveNumber;
} ChessHalfMove;

/* Structure to represent the chessgame */
typedef struct
{
    char*           pgn;
    ChessHalfMove   halfMoves[MAX_FEN_LENGTH];

} ChessGame;

/* Structure to represent the chessboard */
typedef struct{

    char fen[MAX_FEN_LENGTH]; //
    char board[BOARD_SIZE][BOARD_SIZE]; // état de la table
    char turn; // à qui de jouer
    char castling[4]; // roque
    char en_passant[2]; // en passant
    int halfmove_clock; // halfmove clock
    int fullmove_number; // each 2 halfmoves clock

} postgres_chessboard;


