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
    return c;
}

static ChessGame * chessGame_make(char *pgn)
{
    ChessGame *c = palloc0(sizeof(ChessGame));
    c->pgn = pgn;
    memcpy(c->moves, chessMove_parse(pgn), sizeof(c->moves));
    return c;
}

static ChessMove * chessMove_parse(char *pgn)
{
    ChessMove *chessMoves[MAX_PGN_LENGTH]; // Array of chess moves
    char*  fm, sm;
    int nmb;

    char copy[strlen(pgn) + 1];
    strcpy(copy, pgn);

    char *token = strtok(copy, " "); // Tokenize by spaces
    int moveNumber = 1;

    int parsecount = 0;

    while (token != NULL) {
        if (strstr(token, ".") != NULL) { // Check if token contains a dot
            moveNumber = atoi(token); // Extract move number
        } 
        else if (parsecount == 0) {
            fm = token; // Extract first half move
            parsecount++;
        }
        else if (parsecount == 1) {
            sm = token; // Extract second half move
            parsecount = 0;
        }
        token = strtok(NULL, " ");
        chessMoves[moveNumber-1] = chessMove_make(fm, sm, moveNumber);
    }

    return chessMoves;
}






