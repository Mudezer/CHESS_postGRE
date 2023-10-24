#include <postgres.h>



/************************************************************************/

/* Structure to represent the chessgame */
typedef struct
{
    string      pgn,
    ChessMove[] moves;

} ChessGame;

typedef struct
{
    string      move,
    int        moveNumber;
} ChessMove;

