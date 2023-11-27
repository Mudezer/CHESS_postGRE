/* 
 * chess.h 
 */
#include "smallchesslib.h"

#define BOARD_SIZE 8
#define SCL_BOARD_SQUARES 64
#define MAX_FEN_LENGTH 100
#define MAX_PGN_LENGTH 100
#define MAX_MOVE_LENGTH 7
#define SCL_BOARD_STATE_SIZE 69



/*****************************************************************************
 * Structures
 *****************************************************************************/

// Structure to represent the chess game
typedef struct
{
    char*           pgn;
    SCL_Record      record;

} ChessGame;


// Structure to represent the chessboard
typedef struct{

    char            fen[MAX_FEN_LENGTH];
    SCL_Board       board;

} ChessBoard;

