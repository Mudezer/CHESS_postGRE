/* 
 * chess.h 
 */

#define BOARD_SIZE 8
#define SCL_BOARD_SQUARES 64
#define MAX_FEN_LENGTH 100
#define MAX_PGN_LENGTH 100
#define MAX_MOVE_LENGTH 7
#define SCL_BOARD_STATE_SIZE 69


/* Structure to represent a  move */
typedef struct
{
    char        firstHalfMove[MAX_MOVE_LENGTH];
    char        secondHalfMove[MAX_MOVE_LENGTH];
    int         moveNumber;
} ChessMove;

/* Structure to represent the chessgame */ // Record in smallchesslib.h? -> only the moves parts
typedef struct
{
    char*           pgn;
    ChessMove*      moves;

} ChessGame;

/* Structure to represent the chessboard */ // Board in smallchesslib.h?
typedef struct{

    char fen[MAX_FEN_LENGTH]; //
    char board[BOARD_SIZE][BOARD_SIZE]; // état de la table
    char turn; // à qui de jouer
    char castling[4]; // roque
    char en_passant[2]; // en passant
    int halfmove_clock; // halfmove clock
    int fullmove_number; // each 2 halfmoves clock

} postgres_chessboard;

