/*
 * chess.c
 */

//#include "postgres.h"
/*#include "postgres.h"*/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

/*#include <smallchesslib.h>
#include "utils/builtins.h"
#include "libpq/pqformat.h"
*/

PG_MODULE_MAGIC;

#define BOARD_SIZE 8
#define BOARD_SIZE_SQUARE 64
#define MAX_FEN_LENGTH 100

/*****************************************************************************/

/* Structure to represent the chessgame */
/*
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
*/


typedef struct{
    char *fen; // TODO fen[MAX_FEN_LENGTH];
    char *turn; // turn à qui de jouer
    char *castling; // castling[4] roque
    char *en_passant; // en_passant[2]
    int halfmove_clock; // halfmove clock
    int fullmove_number; // each 2 halfmoves clock
} postgres_chessboard2;


/*****************************************************************************/

/*****************************************************************************/

/*****************************************************************************/

static postgres_chessboard2 * postgres_chessboard_make2(char *fen, char *turn, char *castling, char *en_passant, int halfmove_clock, int fullmove_number)
{
  postgres_chessboard2 *c = malloc(sizeof(postgres_chessboard2));
  c->fen = strdup(fen);
  c->turn = strdup(turn);
  c->castling = strdup(castling);
  c->en_passant = strdup(en_passant);
  c->halfmove_clock = halfmove_clock;
  c->fullmove_number = fullmove_number;
        
  return c;
}

/*****************************************************************************/


static postgres_chessboard2 * fen_parse2(char * str)
{

    char *fen; //
    char *turn; // à qui de jouer*/
    char *castling; // roque
    char *en_passant; // en passant
    int halfmove_clock; // halfmove clock*/
    int fullmove_number; // each 2 halfmoves clock

    char* rest[14] = {0}; // 11 char pointers  
    size_t n = 0; 

    /*Parser for elements*/    
    char *ptr = strtok(str, " ");    

    rest[n++] = ptr; 
    printf("'%s'\n", ptr);
    fen = ptr;

    ptr = strtok(NULL, " ");
    rest[n++] = ptr; 
    printf("'%s'\n", ptr);
    turn = ptr;
    
    ptr = strtok(NULL, " ");
    rest[n++] = ptr; 
    printf("'%s'\n", ptr);
    castling = ptr;

    ptr = strtok(NULL, " ");
    rest[n++] = ptr; 
    printf("'%s'\n", ptr);
    en_passant = ptr;

    ptr = strtok(NULL, " "); 
    rest[n++] = ptr; 
    printf("'%s'\n", ptr);
    halfmove_clock = atoi(ptr);

    ptr = strtok(NULL, " "); 
    rest[n++] = ptr; 
    printf("'%s'\n", ptr);
    fullmove_number = atoi(ptr);

    return postgres_chessboard_make2(fen, turn, castling, en_passant, halfmove_clock, fullmove_number);

}





static char * chessboard_to_str(const postgres_chessboard2 *c)
{
  char *str = malloc (sizeof (char) *100);
  sprintf(str, "%s %s %s %s %d %d", c->fen,c->turn, c->castling, c->en_passant, c->halfmove_clock, c->fullmove_number);

  return str;
}


/*****************************************************************************/

/*****************************************************************************/

/*****************************************************************************/


int main()
{
    char str[] = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    postgres_chessboard2 *c;
    c = fen_parse2(str);
        /*printf("'%s'\n", c->fen);
        printf("'%s'\n", c->turn);
        printf("'%s'\n", c->castling);
        printf("'%s'\n", c->en_passant);
        printf("'%d'\n", c->halfmove_clock);
        printf("'%d'\n", c->fullmove_number);*/
   
    char *str2;
    str2 = chessboard_to_str(c);
    printf("'%s'\n", str2);
    return 0;
}


#include "chess.h"

static ChessMove * chessMove_make(char *firstHalfMove,char *secondHalfMove, int moveNumber)
{
    ChessMove *move = malloc(sizeof(ChessMove));
    strcpy(move->firstHalfMove, firstHalfMove);
    strcpy(move->secondHalfMove, secondHalfMove);
    move->moveNumber = moveNumber;

    return move;
}

void print_moves(ChessMove *moves) //for debug purposes
{
    for(int i = 0; i < 4; i++)
    {
        printf("%d\n", moves[i].moveNumber);
        printf("%s\n", moves[i].firstHalfMove);
        printf("%s\n", moves[i].secondHalfMove);
    }
}

static ChessGame * chessGame_make(char *pgn, ChessMove* moves)
{
    ChessGame *game = (ChessGame*)malloc(sizeof(ChessMove)*MAX_PGN_LENGTH+sizeof(ChessGame));
    game->pgn = pgn;
    game->moves = moves;
    
    return game;
}

static ChessGame * chessMove_parse(char *pgn)
{
    ChessMove* chessMoves = (ChessMove*)malloc(sizeof(ChessMove)*MAX_PGN_LENGTH); // Array of chess moves
    char*   fm; 
    char*   sm;
    int nmb;

    char copy[strlen(pgn) + 1];
    strcpy(copy, pgn);

    char *token = strtok(copy, " "); // Tokenize by 
    
    int moveIndex = 1;
    int parsecount = 0;

    while (token != NULL) {
        if (strstr(token, ".") != NULL) { // Check if token contains a dot
            moveIndex = atoi(token); // Extract move number
            token = strtok(NULL, " ");
        } 
        if (parsecount == 0) {
            fm = token; // Extract first half move
            parsecount++;
            token = strtok(NULL, " ");
        }
        if (parsecount == 1) {
            sm = token; // Extract second half move
            parsecount = 0;
            token = strtok(NULL, " ");
        }
        chessMoves[moveIndex-1] = *chessMove_make(fm, sm, moveIndex);
        sm = NULL;
    }

    return chessGame_make(pgn, chessMoves);
}

static char * getFirstMoves(ChessGame *game, int n)
{
    char *pgn = malloc(MAX_PGN_LENGTH*MAX_MOVE_LENGTH);
    char str_move[10];

    /* Print of first halfmove */
    strcat(pgn, "1. ");
    char* first_move = game->moves[0].firstHalfMove;
    strcat(pgn, first_move);
    
    if(n == 1) return pgn;
    else 
    {
        /* Print of second halfmove */
        strcat(pgn, " ");
        strcat(pgn, game->moves[0].secondHalfMove);

        for(int i = 2; i < n+1; i++) {
            if(i % 2 == 0) {
                sprintf(str_move," %i. ", (i/2)+1);
                strcat(pgn, str_move);
                strcat(pgn, game->moves[i/2].firstHalfMove);
            }
            else {
                strcat(pgn, " ");
                strcat(pgn, game->moves[i/2].secondHalfMove);
            }   
        }
    }
    return pgn;
}

static int hasOpening(ChessGame * game, ChessGame * opening)
{
    int isSame = 1;
    int openingLength = sizeof(opening->moves)/sizeof(ChessMove);
    
    char *gameOpeningSAN = getFirstMoves(game, openingLength);
    char *openingSAN = getFirstMoves(opening, openingLength);

    if(strcmp(gameOpeningSAN, openingSAN) != 0) isSame = 0;

    return isSame;
}

int main()
{
    const char *pgn = "1. e4 e5 2. Nf3 Nc6 3. Bb5 a6 4. Ba4 Nf6";
    
    ChessGame *game = chessMove_parse(pgn);
    
    printf("%s\n", getFirstMoves(game, 7));

    free(game);

    return 0;
}


/*****COPY*/


/*****************************************************************************/

/* Structure to represent the chessgame */
/*
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
*/


typedef struct{
    char *fen; // TODO fen[MAX_FEN_LENGTH];
    char *turn; // turn à qui de jouer
    char *castling; // castling[4] roque
    char *en_passant; // en_passant[2]
    int halfmove_clock; // halfmove clock
    int fullmove_number; // each 2 halfmoves clock
} postgres_chessboard2;


/*****************************************************************************/

/*****************************************************************************/

/*****************************************************************************/

static postgres_chessboard2 * postgres_chessboard_make2(char *fen, char *turn, char *castling, char *en_passant, int halfmove_clock, int fullmove_number)
{
  postgres_chessboard2 *c = malloc(sizeof(postgres_chessboard2));
  c->fen = strdup(fen);
  c->turn = strdup(turn);
  c->castling = strdup(castling);
  c->en_passant = strdup(en_passant);
  c->halfmove_clock = halfmove_clock;
  c->fullmove_number = fullmove_number;
        
  return c;
}

/*****************************************************************************/


static postgres_chessboard2 * fen_parse2(char * str)
{

    char *fen; //
    char *turn; // à qui de jouer*/
    char *castling; // roque
    char *en_passant; // en passant
    int halfmove_clock; // halfmove clock*/
    int fullmove_number; // each 2 halfmoves clock

    char* rest[14] = {0}; // 11 char pointers  
    size_t n = 0; 

    /*Parser for elements*/    
    char *ptr = strtok(str, " ");    

    rest[n++] = ptr; 
    printf("'%s'\n", ptr);
    fen = ptr;

    ptr = strtok(NULL, " ");
    rest[n++] = ptr; 
    printf("'%s'\n", ptr);
    turn = ptr;
    
    ptr = strtok(NULL, " ");
    rest[n++] = ptr; 
    printf("'%s'\n", ptr);
    castling = ptr;

    ptr = strtok(NULL, " ");
    rest[n++] = ptr; 
    printf("'%s'\n", ptr);
    en_passant = ptr;

    ptr = strtok(NULL, " "); 
    rest[n++] = ptr; 
    printf("'%s'\n", ptr);
    halfmove_clock = atoi(ptr);

    ptr = strtok(NULL, " "); 
    rest[n++] = ptr; 
    printf("'%s'\n", ptr);
    fullmove_number = atoi(ptr);

    return postgres_chessboard_make2(fen, turn, castling, en_passant, halfmove_clock, fullmove_number);

}





static char * chessboard_to_str(const postgres_chessboard2 *c)
{
  char *str = malloc (sizeof (char) *100);
  sprintf(str, "%s %s %s %s %d %d", c->fen,c->turn, c->castling, c->en_passant, c->halfmove_clock, c->fullmove_number);

  return str;
}


/*****************************************************************************/

/*****************************************************************************/

/*****************************************************************************/


int main()
{
    char str[] = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    postgres_chessboard2 *c;
    c = fen_parse2(str);
        /*printf("'%s'\n", c->fen);
        printf("'%s'\n", c->turn);
        printf("'%s'\n", c->castling);
        printf("'%s'\n", c->en_passant);
        printf("'%d'\n", c->halfmove_clock);
        printf("'%d'\n", c->fullmove_number);*/
   
    char *str2;
    str2 = chessboard_to_str(c);
    printf("'%s'\n", str2);
    return 0;
}
/*****COPY1*/

static ChessMove * chessMove_make(char *firstHalfMove,char *secondHalfMove, int moveNumber)
{
    ChessMove *move = malloc(sizeof(ChessMove));
    strcpy(move->firstHalfMove, firstHalfMove);
    strcpy(move->secondHalfMove, secondHalfMove);
    move->moveNumber = moveNumber;

    return move;
}

void print_moves(ChessMove *moves) //for debug purposes
{
    for(int i = 0; i < 4; i++)
    {
        printf("%d\n", moves[i].moveNumber);
        printf("%s\n", moves[i].firstHalfMove);
        printf("%s\n", moves[i].secondHalfMove);
    }
}

static ChessGame * chessGame_make(char *pgn, ChessMove* moves)
{
    ChessGame *game = (ChessGame*)malloc(sizeof(ChessMove)*MAX_PGN_LENGTH+sizeof(ChessGame));
    game->pgn = pgn;
    game->moves = moves;
    
    return game;
}

static ChessGame * chessMove_parse(char *pgn)
{
    ChessMove* chessMoves = (ChessMove*)malloc(sizeof(ChessMove)*MAX_PGN_LENGTH); // Array of chess moves
    char*   fm; 
    char*   sm;
    int nmb;

    char copy[strlen(pgn) + 1];
    strcpy(copy, pgn);

    char *token = strtok(copy, " "); // Tokenize by 
    
    int moveIndex = 1;
    int parsecount = 0;

    while (token != NULL) {
        if (strstr(token, ".") != NULL) { // Check if token contains a dot
            moveIndex = atoi(token); // Extract move number
            token = strtok(NULL, " ");
        } 
        if (parsecount == 0) {
            fm = token; // Extract first half move
            parsecount++;
            token = strtok(NULL, " ");
        }
        if (parsecount == 1) {
            sm = token; // Extract second half move
            parsecount = 0;
            token = strtok(NULL, " ");
        }
        chessMoves[moveIndex-1] = *chessMove_make(fm, sm, moveIndex);
        sm = NULL;
    }

    return chessGame_make(pgn, chessMoves);
}

static char * getFirstMoves(ChessGame *game, int n)
{
    char *pgn = malloc(MAX_PGN_LENGTH*MAX_MOVE_LENGTH);
    char str_move[10];

    /* Print of first halfmove */
    strcat(pgn, "1. ");
    char* first_move = game->moves[0].firstHalfMove;
    strcat(pgn, first_move);
    
    if(n == 1) return pgn;
    else 
    {
        /* Print of second halfmove */
        strcat(pgn, " ");
        strcat(pgn, game->moves[0].secondHalfMove);

        for(int i = 2; i < n+1; i++) {
            if(i % 2 == 0) {
                sprintf(str_move," %i. ", (i/2)+1);
                strcat(pgn, str_move);
                strcat(pgn, game->moves[i/2].firstHalfMove);
            }
            else {
                strcat(pgn, " ");
                strcat(pgn, game->moves[i/2].secondHalfMove);
            }   
        }
    }
    return pgn;
}

static int hasOpening(ChessGame * game, ChessGame * opening)
{
    int isSame = 1;
    int openingLength = sizeof(opening->moves)/sizeof(ChessMove);
    
    char *gameOpeningSAN = getFirstMoves(game, openingLength);
    char *openingSAN = getFirstMoves(opening, openingLength);

    if(strcmp(gameOpeningSAN, openingSAN) != 0) isSame = 0;

    return isSame;
}

int main()
{
    const char *pgn = "1. e4 e5 2. Nf3 Nc6 3. Bb5 a6 4. Ba4 Nf6";
    
    ChessGame *game = chessMove_parse(pgn);
    
    printf("%s\n", getFirstMoves(game, 7));

    free(game);

    return 0;}