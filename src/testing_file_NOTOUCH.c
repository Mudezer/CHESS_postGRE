
// C FUNCTIONS TO TRANSLATE IN SQL TO TEST AVERALL FUNCTION => TEST.SQL

/**
*   TESTS
*/
void test_getFirstMoves() {
    char pgn[MAX_PGN_LENGTH] = "1. e4 e5 2. Bc4 Nf6";
    char expected1[MAX_PGN_LENGTH] = "1. e4";
    char expected2[MAX_PGN_LENGTH] = "1. e4 e5";
    char expected3[MAX_PGN_LENGTH] = "1. e4 e5 2. Bc4";
    char expected4[MAX_PGN_LENGTH] = "1. e4 e5 2. Bc4 Nf6";

    int cond1 = strcmp(getFirstMoves(pgn, 1), expected1) == 0;
    int cond2 = strcmp(getFirstMoves(pgn, 2), expected2) == 0;
    int cond3 = strcmp(getFirstMoves(pgn, 3), expected3) == 0;
    int cond4 = strcmp(getFirstMoves(pgn, 4), expected4) == 0;

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

    int cond1 = hasOpening(game, opening1) == 1;
    int cond2 = hasOpening(game, opening2) == 1;
    int cond3 = hasOpening(game, opening3) == 0;

    if(cond1 && cond2 && cond3)
        printf("Test hasOpening passed\n");
    else    
        printf("Test hasOpening failed\n");
}

void test_hasBoard() {
    char pgn[MAX_PGN_LENGTH] ="1. e4 c5 2. Nf3 d6 3. d4 cxd4 4. Nxd4 Nf6 5. Nc3 Nc6 6. Bc4 e6 7. Be3 Be7 8. Bb3 O-O 9. Qe2 Qa5 10. O-O-O Nxd4 11. Bxd4 Bd7 12. Kb1 Bc6 13. f4 Rad8 14. Rhf1 b5 15. f5 b4 16. fxe6 bxc3 17. exf7+ Kh8 18. Rf5 Qb4 19. Qf1 Nxe4 20. a3 Qb7 21. Qf4 Ba4 22. Qg4 Bf6 23. Rxf6 Bxb3";
    char board_FEN[MAX_FEN_LENGTH] = "3r1r1k/pq3Ppp/3p1R2/8/3Bn1Q1/Pbp5/1PP3PP/1K1R4 w - - 0 24";

    int cond1 = hasBoard(pgn, board_FEN, 46) == 1;
    int cond2 = hasBoard(pgn, board_FEN, 45) == 0;

    if(cond1 && cond2)
        printf("Test hasBoard passed\n");
    else    
        printf("Test hasBoard failed\n");
}

void test_getBoard() {
    char pgn[MAX_PGN_LENGTH] ="1. e4 c5 2. Nf3 d6 3. d4 cxd4 4. Nxd4 Nf6 5. Nc3 Nc6 6. Bc4 e6 7. Be3 Be7 8. Bb3 O-O 9. Qe2 Qa5 10. O-O-O Nxd4 11. Bxd4 Bd7 12. Kb1 Bc6 13. f4 Rad8 14. Rhf1 b5 15. f5 b4 16. fxe6 bxc3 17. exf7+ Kh8 18. Rf5 Qb4 19. Qf1 Nxe4 20. a3 Qb7 21. Qf4 Ba4 22. Qg4 Bf6 23. Rxf6 Bxb3";
    char expected1[MAX_FEN_LENGTH] = "rnbqkbnr/pp2pppp/3p4/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R w KQkq - 0 3";
    char expected2[MAX_FEN_LENGTH] = "r1bqkb1r/pp2pppp/2np1n2/8/3NP3/2N5/PPP2PPP/R1BQKB1R w KQkq - 3 6";
    char expected3[MAX_FEN_LENGTH] = "3r1r1k/pq3Ppp/3p1R2/8/3Bn1Q1/Pbp5/1PP3PP/1K1R4 w - - 0 24";
    char falseFEN[MAX_FEN_LENGTH] = "3r1r1k/pq3Ppp/3p1R2/8/3Bn1Q1/Pbp5/1PP3PP/1K1R4 w - - 0 25";

    char* board1 = getBoard(pgn, 4);
    char* board2 = getBoard(pgn, 10);
    char* board3 = getBoard(pgn, 46);

    int cond1 = strcmp(board1, expected1) == 0;
    int cond2 = strcmp(board2, expected2) == 0;
    int cond3 = strcmp(board3, expected3) == 0;
    int cond4 = strcmp(board3, falseFEN) != 0;

    if(cond1 && cond2 && cond3 && cond4)
        printf("Test getBoard passed\n");
    else    
        printf("Test getBoard failed\n");
}