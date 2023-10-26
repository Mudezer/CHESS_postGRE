#include <postgres.h>



/*****************************************************************************/

/* Structure to represent the chessgame */

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

