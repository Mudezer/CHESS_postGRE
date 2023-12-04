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

/* PETER_MON_CRANE.py */
#define PG_GETARG_ChessGame_P(n) DatumGetChessGameP(PG_GETARG_DATUM(n))
 // ça la c'est de la merde, soit on stocke le struct soit on passe le string et on chessmake 
 // le problème c'est que dans le pdf il demande que les fonctions puissent recevoir des chessgame et pas des string donc
 //, le serpent se mord t'il la queue?

#define PG_RETURN_ChessGame_P(x) return ChessGamePGetDatum(x)



/*****************************************************************************
 * Structures
 *****************************************************************************/

// Structure to represent the chess game
typedef struct
{
    char            pgn[MAX_PGN_LENGTH];
    SCL_Record      record;
    ChessBoard*     board[beacouptrop];

} ChessGame;


// Structure to represent the chessboard
typedef struct{

    char            fen[MAX_FEN_LENGTH];
    SCL_Board       board;

} ChessBoard;

