/* 
 * chess.h 
 */

#define BOARD_SIZE 8
#define BOARD_SIZE_SQUARE 64
#define MAX_FEN_LENGTH 100
#define MAX_PGN_LENGTH 100
#define MAX_MOVE_LENGTH 7


/* Structure to represent a  move */
typedef struct
{
    char        firstHalfMove[MAX_MOVE_LENGTH];
    char        secondHalfMove[MAX_MOVE_LENGTH];
    int         moveNumber;
} ChessMove;

/* Structure to represent the chessgame */
typedef struct
{
    char           pgn[MAX_PGN_LENGTH];
    ChessMove      moves[MAX_PGN_LENGTH];
    //ChessBoard     boards[MAX_PGN_LENGTH];

} ChessGame;

/* Structure to represent the chessboard */
typedef struct{

    char fen[MAX_FEN_LENGTH];
    char board[BOARD_SIZE][BOARD_SIZE]; // état de la table
    char turn; // à qui de jouer
    char castling[4]; // roque
    char en_passant[2]; // en passant
    int halfmove_clock; // halfmove clock
    int fullmove_number; // each 2 halfmoves clock

} ChessBoard;

typedef struct{
    char    move[10];
    int     halfMoveNumber;
    char    movedPiece;
    int     endLocation[2];
    int     startLocation[2];
    int     capture ;
    int     kingSideCastling;
    int     queenSideCastling;
    int     check;
    int     pawnPromotion;
    int     checkmate;
} ChessHalfMove;
