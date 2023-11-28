/*
 * chess.c
 */

/*#include <postgres.h>*/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include "chess.h"

#include "smallchesslib.h"
/*#include "utils/builtins.h"
#include "libpq/pqformat.h"*/


PG_MODULE_MAGIC;

#define BOARD_SIZE 8
#define SCL_BOARD_SQUARES 64 /*#define SCL_BOARD_SQUARES 64*/
#define MAX_FEN_LENGTH 100

/*****************************************************************************/


/*static postgres_chessboard * 
postgres_chessboard_make(char *fen, char *state, char *turn, char *castling, char *en_passant, int halfmove_clock, int fullmove_number)
{
    // struct building phase
    postgres_chessboard *c = malloc(sizeof(postgres_chessboard));
    strcpy(c->fen, fen);
    strcpy(c->state, state);
    strcpy(c->turn, turn);
    strcpy(c->castling, castling);
    strcpy(c->en_passant, en_passant);
    c->halfmove_clock = halfmove_clock;
    c->fullmove_number = fullmove_number;
    return c;
}



static postgres_chessboard * fen_parse(char * str) // on pourrati eventuellement le rename en postgres_chessboard_from_str
{

    //parsing phase

    char *state;
    char *turn;
    char *castling;
    char *en_passant; 
    int halfmove_clock;
    int fullmove_number;

    char* rest[14] = {0}; // 11 char pointers  
    size_t n = 0; 

    char fen[strlen(str) + 1];
    strcpy(fen, str);
    printf("'%s'\n", fen);

    //Parser for elements
    char *ptr = strtok(str, " ");    
    rest[n++] = ptr; 
    printf("'%s'\n", ptr);
    state = ptr;

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

    // return directly the struct
    return postgres_chessboard_make2(fen, state, turn, castling, en_passant, halfmove_clock, fullmove_number);
}


static char * chessboard_to_str(const postgres_chessboard *c)
{
  char *str = malloc (sizeof (char) *200);
  sprintf(str, "%s %s %s %s %s %d %d", c->fen, c->state, c->turn, c->castling, c->en_passant, c->halfmove_clock, c->fullmove_number);

  return str;
}
*/



/*****************************************************************************/

/*****************************************************************************/



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


static char chessboard_to_fen(const postgres_chessboard *c){ return c->fen;}

/*****************************************************************************/

/**
 * The following functions are the link between the SQL and the C code
*/
Datum chessboard_in(PG_FUNCTION_ARGS);
Datum chessboard_out(PG_FUNCTION_ARGS); // d'abord on déclare la fonction

PG_FUNCTION_INFO_V1(chessboard_in); // lien entre SQL et C
Datum chessboard_in(PG_FUNCTION ARGS){
    char *str = PG_GETARG_CSTRING(0); // on récupère le string entrant du coté SQL
    PG_RETURN_POINTER(fen_parse2(str)); // on renvoie le pointeur vers le struct C
    // à noté que le coté SQL s'en fout du type de pointeur qu'on renvoie du moment qu'on lui indique que c'est un pointeur
}

PG_FUNCTION_INFO_V1(fen_out); // lien entre SQL et C
Datum fen_out(PG_FUNCTION_ARGS){ // Datum c'est la manière de définir un data type générique
    // ici on récupère le pointeur vers le SQL datatype du chessboard et on le cast en pointeur du struct C
    // en bref la ligne suivante fait une conversion SQL datatype => struct C
    postgres_chessboard *c = (postgres_chessboard *)PG_GETARG_POINTER(0); 
    PG_RETURN_CSTRING(chessboard_to_fen(c)); // ici on récupère le string 'fen' sortant du struct et on l'envoie du coté SQL
}

// Chessboard's Getters


Datum getBoard(PG_FUNCTION_ARGS);
Datum getTurn(PG_FUNCTION_ARGS);
Datum getCastling(PG_FUNCTION_ARGS);
Datum getEnPassant(PG_FUNCTION_ARGS);
Datum getHalfMoveClock(PG_FUNCTION_ARGS);
Datum getFullMoveNumber(PG_FUNCTION_ARGS);
Datum chessboardToString(PG_FUNCTION_ARGS);

PG_FUNCTION_INFO_V1(getBoard);
Datum getBoard(PG_FUNCTION_ARGS){
    postgres_chessboard *c = (postgres_chessboard *)PG_GETARG_POINTER(0);
    PG_RETURN_CSTRING(c->state);    
}

// Chessgame's Getters

Datum getFirstMoves(PG_FUNCTION_ARGS);







int main()
{
    // Test the FEN to Struct
    char str[] = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq e3 0 1";
    postgres_chessboard *c;
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

    // Test the parse of the moves
    const char *pgn = "1. e4 e5 2. Nf3 Nc6 3. Bb5 a6 4. Ba4 Nf6";
    
    ChessGame *game = chessMove_parse(pgn);
    
    printf("%s\n", getFirstMoves(game, 7));

    free(game);

    return 0;
}

/**
 * 
*/
