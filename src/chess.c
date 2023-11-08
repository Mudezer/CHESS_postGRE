/*
 * chess.c
 */

//#include "postgres.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

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