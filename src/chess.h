/* 
 * chess.h 
 */

#include "smallchesslib.h"

#define BOARD_SIZE 8
#define SCL_BOARD_SQUARES 64
#define MAX_FEN_LENGTH 100
#define MAX_PGN_LENGTH 100 * MAX_MOVE_LENGTH
#define MAX_MOVE_LENGTH 7
#define SCL_BOARD_STATE_SIZE 69


/* fmgr macros chessboard type */ //TODO put them in chess.h

#define DatumGetChessBoardP(X)  ((ChessBoard *) DatumGetPointer(X))
#define ChessBoardPGetDatum(X)  PointerGetDatum(X)
#define PG_GETARG_ChessBoard_P(n) DatumGetChessBoardP(PG_GETARG_DATUM(n))
#define PG_RETURN_ChessBoard_P(x) return ChessBoardPGetDatum(x)

#define DatumGetChessGameP(X)  ((ChessGame *) DatumGetPointer(X))
#define ChessGamePGetDatum(X)  PointerGetDatum(X)
#define PG_GETARG_ChessGame_P(n) DatumGetChessGameP(PG_GETARG_DATUM(n))
#define PG_RETURN_ChessGame_P(x) return ChessGamePGetDatum(x)



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

