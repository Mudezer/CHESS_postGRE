/*
 * chess.c
 */

#include "postgres.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "chess.h"

static ChessMove * chessMove_make(char *firstHalfMove,char *secondHalfMove, int moveNumber)
{
    ChessMove *c = palloc0(sizeof(ChessMove));
    c->firstHalfMove = firstHalfMove;
    c->secondHalfMove = secondHalfMove;
    c->moveNumber = moveNumber;
    // printf("%s\n", c->firstHalfMove);
    // printf("%s\n", c->secondHalfMove);
    return c;
}

void print_moves(ChessMove *moves) //for debug purposes
{
    for(int i = 0; i < 4; i++)
    {
        printf("%s\n", moves[i].firstHalfMove);
        printf("%s\n", moves[i].secondHalfMove);
    }
}

static ChessGame * chessGame_make(char *pgn, ChessMove* moves)
{
    ChessGame *c = (ChessGame*)palloc0(sizeof(ChessMove)*MAX_PGN_LENGTH+sizeof(ChessGame));
    c->pgn = pgn;
    //print_moves(moves);
    c->moves = moves;
    //print_moves(c->moves);
    //printf("%s\n", c->moves[2].firstHalfMove);
    //printf("%s\n", c->moves[2].secondHalfMove);
    return c;
}

static ChessGame * chessMove_parse(char *pgn)
{
    ChessMove* chessMoves = (ChessMove*)palloc0(sizeof(ChessMove)*MAX_PGN_LENGTH); // Array of chess moves
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
        // printf("%d\n", moveIndex);
        // printf("%s\n", fm);
        // printf("%s\n", sm);
        chessMoves[moveIndex-1] = *chessMove_make(fm, sm, moveIndex);
        sm = NULL;
    }

    return chessGame_make(pgn, chessMoves);
}

static char * getFirstMoves(ChessGame *game, int n)
{
    char *pgn = malloc(MAX_PGN_LENGTH);
    char str_move[10];
    //printf("step 1\n");
    strcat(pgn, "1. ");
    printf("%s\n", pgn);

    //printf("step 2\n");
    char* tmp = game->moves[0].firstHalfMove;
    printf("%s\n", tmp);
    strcat(pgn, tmp);
    printf("%s\n", pgn);
    //("%s",game->moves[0].firstHalfMove);
    if(n == 1) return pgn;
    else 
    {
        strcat(pgn, " ");
        //printf("step 4\n");
        strcat(pgn, game->moves[0].secondHalfMove);
        printf("%s",game->moves[0].secondHalfMove);
        printf("step 5\n");
        for(int i = 2; i < n+1; i++) {
            sprintf(str_move,"%d. ", i/2);
            printf("step \n");
            strcat(pgn, str_move);
            printf("step 7\n");
            strcat(pgn, game->moves[i/2].firstHalfMove);
            printf("step 8\n");
            if(i % 2 != 0)
                strcat(pgn, " ");
                printf("step 9\n");
                strcat(pgn, game->moves[i/2].secondHalfMove);
                printf("step 10\n");
        }
    }
    return pgn;
}

int main()
{
    printf("test init\n");
    const char *pgn = "1. e4 e5 2. Nf3 Nc6 3. Bb5 a6 4. Ba4 Nf6";
    
    ChessGame *game = chessMove_parse(pgn);
    print_moves(game->moves);
    
    //printf("%s\n", game->moves[2].secondHalfMove);
    //printf("%s\n", game->moves[2].firstHalfMove);
    printf("%s\n", pgn);
    printf("%s\n", pgn);
    //printf("%s\n", game->moves[2].firstHalfMove);

    
    //printf("%s\n", getFirstMoves(game, 4));
    free(game);
    return 0;
}