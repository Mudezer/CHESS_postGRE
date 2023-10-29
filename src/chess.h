/* 
 * chess.h 
 */

#define BOARD_SIZE 8
#define BOARD_SIZE_SQUARE 64
#define MAX_FEN_LENGTH 100
#define MAX_PGN_LENGTH 100


/* Structure to represent a  move */
typedef struct
{
    char*       firstHalfMove;
    char*       secondHalfMove;
    int         moveNumber;
} ChessMove;

/* Structure to represent the chessgame */
typedef struct
{
    char*           pgn;
    ChessMove       moves[MAX_PGN_LENGTH];

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