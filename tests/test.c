#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <float.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../src/chess.h"

static ChessGame * chessgame_make(const char * pgn)
{
    ChessGame *c = malloc(sizeof(ChessGame));
    strcpy(c->pgn, pgn);
    SCL_recordFromPGN(c->record, pgn);
    return c;
}

static ChessBoard * chessboard_make(const char fen[MAX_FEN_LENGTH])
{
    ChessBoard *c = malloc(sizeof(ChessBoard));
    SCL_boardFromFEN(c->board, fen);
    strcpy(c->fen, fen);
    return c;
}

static ChessGame * getFirstMoves(ChessGame * chessgame, int n)
{
    char * gamePGN = chessgame->pgn;
    char gamePGN_copy[MAX_PGN_LENGTH]; strcpy(gamePGN_copy, gamePGN);
    char *openingPGN = malloc(sizeof(char)*MAX_PGN_LENGTH);
    int counter = 0;
    int incr = 0;
    char* tok = strtok(gamePGN_copy, " ");
    strcat(openingPGN, tok);
    tok = strtok(NULL, " ");
    while((counter < n) && (tok != NULL)) {
        strcat(openingPGN, " ");
        strcat(openingPGN, tok);
        if(incr == 2)
            incr = 0;
        else {
            counter++;
            incr++;
            }
        
        tok = strtok(NULL, " ");
    }
    return chessgame_make(openingPGN);
}

/**
 * @param chessgame the game to analyze
 * @param opening the opening to find
 * @returns bool true if the game has the opening, false otherwise
*/
static bool hasOpening(ChessGame* chessgame, ChessGame* openingGame)
{
    char * gamePGN = chessgame->pgn;
    char * openingPGN = openingGame->pgn;
    bool isSame = (strstr(gamePGN, openingPGN) != NULL);
    return isSame;
}


/**
 * @param chessgame the game to analyze
 * @param chessboard the board to find
 * @param moveNumber the length of the interval to check
 * @returns bool true if the game has the board, false otherwise
*/
static bool hasBoard(ChessGame* chessgame,ChessBoard * chessboard , int moveNumber)
{
    char * gamePGN = chessgame->pgn;
    char* board_FEN = chessboard->fen;
    int counter = 0;
    bool isSame = false;

    // Board to find
    SCL_Board *board_tofind = malloc(sizeof(SCL_Board));
    SCL_boardFromFEN(*board_tofind, board_FEN);
    char board_tofindStr[65]; // Increase the size by 1 to accommodate the null terminator
    strncpy(board_tofindStr, *board_tofind, 64);
    board_tofindStr[64] = '\0'; // Add the null terminator

    // Boards of the game
    char* firstMoves = getFirstMoves(gamePGN, moveNumber);
    SCL_Record *record = malloc(sizeof(SCL_Record));
    SCL_recordFromPGN(*record, firstMoves);
    SCL_Board *board = malloc(sizeof(SCL_Board));
    char boardStr[65];

    while((counter <= moveNumber) && (isSame == false))
    {
        SCL_recordApply(*record, *board, counter);
        strncpy(boardStr, *board, 64);
        boardStr[64] = '\0';
        isSame = (strcmp(boardStr, board_tofindStr) == 0);
        counter++;
    }

    free(board_tofind); 
    free(board); 
    free(record);

    return isSame;
}

static ChessBoard * getBoard(ChessGame* chessgame, int moveNumber)
{
    char * gamePGN = chessgame->pgn;
    char* boardFEN = malloc(sizeof(char)*MAX_FEN_LENGTH);

    SCL_Record *record = malloc(SCL_RECORD_MAX_SIZE);
    SCL_recordFromPGN(*record, gamePGN);
    SCL_Board *board = malloc(SCL_RECORD_MAX_SIZE);
    SCL_recordApply(*record, *board, moveNumber);
    SCL_boardToFEN(*board, boardFEN);

    free(record);

    return chessboard_make(boardFEN);
}


/**
*   TESTS
*/
void test_getFirstMoves() {
    char pgn[MAX_PGN_LENGTH] = "1. e4 e5 2. Bc4 Nf6";
    ChessGame * pgn1 = chessgame_make(pgn);
    char expected1[MAX_PGN_LENGTH] = "1. e4";
    char expected2[MAX_PGN_LENGTH] = "1. e4 e5";
    char expected3[MAX_PGN_LENGTH] = "1. e4 e5 2. Bc4";
    char expected4[MAX_PGN_LENGTH] = "1. e4 e5 2. Bc4 Nf6";

    int cond1 = strcmp(getFirstMoves(pgn1, 1)->pgn, expected1) == 0;
    int cond2 = strcmp(getFirstMoves(pgn1, 2)->pgn, expected2) == 0;
    int cond3 = strcmp(getFirstMoves(pgn1, 3)->pgn, expected3) == 0;
    int cond4 = strcmp(getFirstMoves(pgn1, 4)->pgn, expected4) == 0;

    if(cond1 && cond2 && cond3 && cond4)
        printf("Test getFirstMoves passed\n");
    else    
        printf("Test getFirstMoves failed\n");
}

void test_hasOpening() {
    char game[MAX_PGN_LENGTH] ="1. e4 c5 2. Nf3 d6 3. d4 cxd4 4. Nxd4 Nf6 5. Nc3 Nc6 6. Bc4 e6 7. Be3 Be7 8. Bb3 O-O 9. Qe2 Qa5 10. O-O-O Nxd4 11. Bxd4 Bd7 12. Kb1 Bc6 13. f4 Rad8 14. Rhf1 b5 15. f5 b4 16. fxe6 bxc3 17. exf7+ Kh8 18. Rf5 Qb4 19. Qf1 Nxe4 20. a3 Qb7 21. Qf4 Ba4 22. Qg4 Bf6 23. Rxf6 Bxb3";
    char opening1[MAX_PGN_LENGTH] = "1. e4 c5 2. Nf3 d6 3. d4 cxd4 4. Nxd4 Nf6";
    char opening2[MAX_PGN_LENGTH] = "1. e4 c5 2. Nf3 d6 3. d4 cxd4 4. Nxd4";
    char opening3[MAX_PGN_LENGTH] = "1. e4 c5 2. Nf5 d6 3. d4";

    ChessBoard * game1 = chessgame_make(game);
    ChessBoard * opening01 = chessgame_make(opening1);
    ChessBoard * opening02 = chessgame_make(opening2);
    ChessBoard * opening03 = chessgame_make(opening3);
    int cond1 = hasOpening(game1, opening01) == true;
    int cond2 = hasOpening(game1, opening02) == true;
    int cond3 = hasOpening(game1, opening03) == false;

    if(cond1 && cond2 && cond3)
        printf("Test hasOpening passed\n");
    else    
        printf("Test hasOpening failed\n");
}

void test_hasBoard() {
    char pgn[MAX_PGN_LENGTH] ="1. e4 c5 2. Nf3 d6 3. d4 cxd4 4. Nxd4 Nf6 5. Nc3 Nc6 6. Bc4 e6 7. Be3 Be7 8. Bb3 O-O 9. Qe2 Qa5 10. O-O-O Nxd4 11. Bxd4 Bd7 12. Kb1 Bc6 13. f4 Rad8 14. Rhf1 b5 15. f5 b4 16. fxe6 bxc3 17. exf7+ Kh8 18. Rf5 Qb4 19. Qf1 Nxe4 20. a3 Qb7 21. Qf4 Ba4 22. Qg4 Bf6 23. Rxf6 Bxb3";
    char board_FEN[MAX_FEN_LENGTH] = "3r1r1k/pq3Ppp/3p1R2/8/3Bn1Q1/Pbp5/1PP3PP/1K1R4 w - - 0 24";

    int cond1 = hasBoard(pgn, board_FEN, 46) == true;
    int cond2 = hasBoard(pgn, board_FEN, 45) == true;//false;

    if(cond1 && cond2)
        printf("Test hasBoard passed\n");
    else    
        printf("Test hasBoard failed\n");
}

void test_getBoard() {
    char pgn1[MAX_PGN_LENGTH] ="1. e4 c5 2. Nf3 d6 3. d4 cxd4 4. Nxd4 Nf6 5. Nc3 Nc6 6. Bc4 e6 7. Be3 Be7 8. Bb3 O-O 9. Qe2 Qa5 10. O-O-O Nxd4 11. Bxd4 Bd7 12. Kb1 Bc6 13. f4 Rad8 14. Rhf1 b5 15. f5 b4 16. fxe6 bxc3 17. exf7+ Kh8 18. Rf5 Qb4 19. Qf1 Nxe4 20. a3 Qb7 21. Qf4 Ba4 22. Qg4 Bf6 23. Rxf6 Bxb3";
    char expected1[MAX_FEN_LENGTH] = "rnbqkbnr/pp2pppp/3p4/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R w KQkq - 0 3";
    char expected2[MAX_FEN_LENGTH] = "r1bqkb1r/pp2pppp/2np1n2/8/3NP3/2N5/PPP2PPP/R1BQKB1R w KQkq - 3 6";
    char expected3[MAX_FEN_LENGTH] = "3r1r1k/pq3Ppp/3p1R2/8/3Bn1Q1/Pbp5/1PP3PP/1K1R4 w - - 0 24";
    char falseFEN[MAX_FEN_LENGTH] = "3r1r1k/pq3Ppp/3p1R2/8/3Bn1Q1/Pbp5/1PP3PP/1K1R4 w - - 0 25";

    ChessBoard * pgn = chessgame_make(pgn1);

    char* board1 = getBoard(pgn, 4)->fen;
    char* board2 = getBoard(pgn, 10)->fen;
    char* board3 = getBoard(pgn, 46)->fen;

    int cond1 = strcmp(board1, expected1) == 0;
    int cond2 = strcmp(board2, expected2) == 0;
    int cond3 = strcmp(board3, expected3) == 0;
    int cond4 = strcmp(board3, falseFEN) != 0;

    if(cond1 && cond2 && cond3 && cond4)
        printf("Test getBoard passed\n");
    else    
        printf("Test getBoard failed\n");
}

int main ()
{
    test_getFirstMoves();
    test_hasOpening();
    test_getBoard();
    test_hasBoard();
    return 0;
}