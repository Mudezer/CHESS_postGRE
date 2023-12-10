/* 
 * chess.h 
 */

#include "smallchesslib.h"

#define MAX_FEN_LENGTH 100
#define MAX_PGN_LENGTH 200 * MAX_MOVE_LENGTH
#define MAX_MOVE_LENGTH 7

#define PG_GETARG_ChessBoard_P(n) ((ChessBoard *) DatumGetPointer(PG_GETARG_DATUM(n)))
#define PG_RETURN_ChessBoard_P(x) return PointerGetDatum(x)

#define PG_GETARG_ChessGame_P(n) ((ChessGame *) DatumGetPointer(PG_GETARG_DATUM(n)))
#define PG_RETURN_ChessGame_P(x) return PointerGetDatum(x)



/*****************************************************************************
 * Structures
 *****************************************************************************/

/* Structure to represent the chess game */
typedef struct
{
    char            pgn[MAX_PGN_LENGTH];
    SCL_Record      record;
} ChessGame;

/* Structure to represent the chessboard */
typedef struct{
    char            fen[MAX_FEN_LENGTH];
    SCL_Board       board;
} ChessBoard;

