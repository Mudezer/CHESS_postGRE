/*
 * chess.c
 */

//#include "postgres.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <float.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include "chess.h"
#include "smallchesslib.h"
//#include "utils/builtins.h"
//#include "libpq/pqformat.h"


PG_MODULE_MAGIC;

#define BOARD_SIZE 8
#define BOARD_SIZE_SQUARE 64
#define MAX_FEN_LENGTH 100


/*****************************************************************************/

/**
 * Constructor of a ChessBoard
*/
static ChessBoard * chessBoard_make(char *fen, char *board, char *turn, char *castling, char *en_passant, int halfmove_clock, int fullmove_number)
{
    // struct building phase
    ChessBoard *c = malloc(sizeof(ChessBoard));
    strcpy(c->fen, fen);
    strcpy(c->board, board);
    strcpy(c->turn, turn);
    strcpy(c->castling, castling);
    strcpy(c->en_passant, en_passant);
    c->halfmove_clock = halfmove_clock;
    c->fullmove_number = fullmove_number;
    return c;
}


/**
 * Parse an input that has a FEN format to create a ChessBoard struct
 * @param fen : the FEN format input
 * @return a pointer to the ChessBoard struct
*/
static ChessBoard * fen_parse(char * fen) // on pourrati eventuellement le rename en postgres_chessboard_from_str
{

    //parsing phase

    char *board;
    char *turn;
    char *castling;
    char *en_passant; 
    int halfmove_clock;
    int fullmove_number;

    char* rest[14] = {0}; // 11 char pointers  
    size_t n = 0; 

    char fencpy[strlen(fen) + 1];
    strcpy(fencpy, fen);
    printf("'%s'\n", fencpy);

    /*Parser for elements*/    
    char *ptr = strtok(fencpy, " ");    
    rest[n++] = ptr; 
    printf("'%s'\n", ptr);
    board = ptr;

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
    return chessBoard_make(fen, board, turn, castling, en_passant, halfmove_clock, fullmove_number);
}

/**
 * Getter of the FEN format of a ChessBoard
 * @param c : the ChessBoard struct
 * @return the FEN format of the ChessBoard
*/
static char * chessboard_to_str(const ChessBoard *c)
{
  char *str = malloc (sizeof (char) *200);
  sprintf(str, "%s %s %s %s %s %d %d", c->fen, c->board, c->turn, c->castling, c->en_passant, c->halfmove_clock, c->fullmove_number);

  return str;
} //??? Pourquoi pas un simple getter du FEN -> chessboard->fen



/*****************************************************************************/

/*****************************************************************************/


/**
 * Constructor of a ChessMove
*/
static ChessMove * chessMove_make(char *firstHalfMove,char *secondHalfMove, int moveNumber)
{
    ChessMove *move = malloc(sizeof(ChessMove));
    strcpy(move->firstHalfMove, firstHalfMove);
    strcpy(move->secondHalfMove, secondHalfMove);
    move->moveNumber = moveNumber;

    return move;
}



/*  DEBUG FUNCTION

void print_moves(ChessMove *moves)
{
    for(int i = 0; i < 4; i++)
    {
        printf("%d\n", moves[i].moveNumber);
        printf("%s\n", moves[i].firstHalfMove);
        printf("%s\n", moves[i].secondHalfMove);
    }
}*/


/**
 * Constructor of a ChessGame
*/
static ChessGame * chessGame_make(char *pgn, ChessMove* moves)
{
    ChessGame *game = (ChessGame*)malloc(sizeof(ChessGame));
    strcpy(game->pgn, pgn);
    strcpy(game->moves, moves);
    //strcpy(game->boards, boards);
    
    return game;
}


/**
 * Parse an input that has a PGN format to create a ChessGame struct
 * @param pgn : the PGN format input
 * @return a pointer to the ChessGame struct
*/
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


/**
 * @param game the game to analyze
 * @param n the number of moves to print
 * @returns  n first moves of a ChessGame in PGN format
*/
static char * getFirstMoves(char* gamePGN, int n)
{
    char* gamePGN_copy; strcpy(gamePGN_copy, gamePGN);
    char* openingPGN;
    int counter = 0;
    char* tok = strtok(gamePGN_copy, " ");
    while((counter < n) && (tok != NULL)) {
        strcat(openingPGN, tok);
        counter++;
        tok = strtok(NULL, " ");
    }
    return openingPGN;
}



/**
 * @param game the game to analyze
 * @param opening the opening to find
 * @returns 1 if the game has the opening, 0 otherwise
*/
static int hasOpening(ChessGame * game, ChessGame * opening)
{
    int isSame = 1;
    int openingLength = sizeof(opening->moves)/sizeof(ChessMove);
    
    char *gameOpeningSAN = getFirstMoves(game, openingLength);
    char *openingSAN = getFirstMoves(opening, openingLength);

    if(strcmp(gameOpeningSAN, openingSAN) != 0) isSame = 0;

    return isSame;
}


/**¨
 * @param game the game to analyze
 * @param board the board to find
 * @param moveNumber the length of the interval to check
 * @returns 1 if the game has the board, 0 otherwise
*/
static int hasBoard(char* gamePGN, char* board_FEN, int moveNumber)
{
    int counter = 0;
    int isSame = 0;

    // Board to find
    SCL_Board *board_tofind = malloc(sizeof(SCL_Board));
    SCL_boardFromFEN(*board_tofind, board_FEN);
    char board_tofindStr[65];
    strncpy(board_tofindStr, *board_tofind, 64);

    // Boards of the game
    char* firstMoves = getFirstMoves(gamePGN, moveNumber);
    SCL_Record *record = malloc(sizeof(SCL_Record));
    SCL_recordFromPGN(*record, firstMoves);
    SCL_Board *board = malloc(sizeof(SCL_Board));
    char boardStr[65];

    while(counter < moveNumber)
    {
        counter++;
        SCL_recordApply(*record, *board, counter);
        if(strcmp(boardStr, board_tofindStr) == 0) isSame = 1;
    }

    free(board_tofind); 
    free(board); 
    free(record);

    return isSame;
}


/**
 * Getter of the board at a specific halfmove
*/
static char * getBoard(char* gamePGN, int moveNumber)
{
    char boardFEN[MAX_FEN_LENGTH];

    SCL_Record *record = malloc(sizeof(SCL_Record));
    SCL_recordFromPGN(*record, gamePGN);
    SCL_Board *board = malloc(sizeof(SCL_Board));
    SCL_recordApply(*record, *board, moveNumber);
    SCL_boardToFEN(*board, boardFEN);

    free(record);
    free(board);

    return boardFEN;
}



static char chessboard_to_fen(const ChessBoard *c){ return c->fen;}



void initChessHalfMove(ChessHalfMove *h){
    h->capture = 0;
    h->check = 0;
    h->checkmate = 0;
    h->kingSideCastling = 0;
    h->queenSideCastling = 0;
    h->endLocation[0] = 0;
    h->endLocation[1] = 0;
    h->startLocation[0] = 0;
    h->startLocation[1] = 0;
    h->pawnPromotion = 0;
}

void setEndLocation(ChessHalfMove *halfMove, char position[]){
    for (int i = 0; i < 2; i++) {
        if (position[i] == 'a') {
            halfMove->endLocation[0] = 1;
        } else if (position[i] == 'b') {
            halfMove->endLocation[0] = 2;
        } else if (position[i] == 'c') {
            halfMove->endLocation[0] = 3;
        } else if (position[i] == 'd') {
            halfMove->endLocation[0] = 4;
        } else if (position[i] == 'e') {
            halfMove->endLocation[0] = 5;
        } else if (position[i] == 'f') {
            halfMove->endLocation[0] = 6;
        } else if (position[i] == 'g') {
            halfMove->endLocation[0] = 7;
        } else if (position[i] == 'h') {
            halfMove->endLocation[0] = 8;
        } else {
            halfMove->endLocation[1] =  position[i] - '0';
        }        
    }
}

void setStartLocation(ChessHalfMove *halfMove, char position[],int moveLength){
    switch (moveLength) {
        case 1:
            if (position[0] == 'a') {
                halfMove->startLocation[0] = 1;
            } else if (position[0] == 'b') {
                halfMove->startLocation[0] = 2;
            } else if (position[0] == 'c') {
                halfMove->startLocation[0] = 3;
            } else if (position[0] == 'd') {
                halfMove->startLocation[0] = 4;
            } else if (position[0] == 'e') {
                halfMove->startLocation[0] = 5;
            } else if (position[0] == 'f') {
                halfMove->startLocation[0] = 6;
            } else if (position[0] == 'g') {
                halfMove->startLocation[0] = 7;
            } else if (position[0] == 'h') {
                halfMove->startLocation[0] = 8;
            } else {
                halfMove->startLocation[1] =  position[0] - '0';
            }
            break;
        case 2:
            for (int i = 0; i < 2; i++) {
                if (position[i] == 'a') {
                    halfMove->startLocation[0] = 1;
                } else if (position[i] == 'b') {
                    halfMove->startLocation[0] = 2;
                } else if (position[i] == 'c') {
                    halfMove->startLocation[0] = 3;
                } else if (position[i] == 'd') {
                    halfMove->startLocation[0] = 4;
                } else if (position[i] == 'e') {
                    halfMove->startLocation[0] = 5;
                } else if (position[i] == 'f') {
                    halfMove->startLocation[0] = 6;
                } else if (position[i] == 'g') {
                    halfMove->startLocation[0] = 7;
                } else if (position[i] == 'h') {
                    halfMove->startLocation[0] = 8;
                } else {
                    halfMove->startLocation[1] =  position[i] - '0';
                }        
            }
            break;
        
    }
}


void halfMoveParser(ChessHalfMove *halfMove) {
    int moveLength = 0;
    while(1){
        if(halfMove->move[moveLength] == '\0'){ break;}
        moveLength++;
    }
    switch (moveLength) {
        case 2:
            halfMove->movedPiece = 'p';
            setEndLocation(halfMove, halfMove->move);
            break;
        case 3:
            if(halfMove->move[0] == 'x')  {
                halfMove->movedPiece = 'p';
                char endLocation[] = {halfMove->move[1],halfMove->move[2]};
                setEndLocation(halfMove,endLocation);
                halfMove->capture = 1;
                break;
            } else if(halfMove->move[2] == '+'){
                halfMove->movedPiece = 'p';
                char endLocation[] = {halfMove->move[0],halfMove->move[1]};
                setEndLocation(halfMove,endLocation);
                halfMove->check = 1;
                break;
            } else if(halfMove->move[2] == '#'){
                halfMove->movedPiece = 'p';
                char endLocation[] = {halfMove->move[0],halfMove->move[1]};
                setEndLocation(halfMove,endLocation);
                halfMove->checkmate = 1;
                break;
            } else if(strcmp(halfMove->move, "O-O") == 0 || strcmp(halfMove->move, "0-0") == 0) {
                halfMove->kingSideCastling = 1;
                break;
            } else if(isupper(halfMove->move[0])) { 
                halfMove->movedPiece = halfMove->move[0];
                char endLocation[] = {halfMove->move[1],halfMove->move[2]};
                setEndLocation(halfMove,endLocation);
                break;
            } else if(isupper(halfMove->move[2])) {
                halfMove->pawnPromotion = 1;
                halfMove->movedPiece = halfMove->move[2];
                char endLocation[] = {halfMove->move[0],halfMove->move[1]};
                setEndLocation(halfMove,endLocation);
                break;
            }
            break;
        case 4:
            if (isupper(halfMove->move[3])){
                halfMove->pawnPromotion = 1;
                halfMove->movedPiece = halfMove->move[3];
                if(halfMove->move[0] == 'x'){
                    halfMove->capture = 1;
                    char endLocation[] = {halfMove->move[1],halfMove->move[2]};
                    setEndLocation(halfMove,endLocation);
                } else {
                    char startLocation[2] = {halfMove->move[0]};
                    setStartLocation(halfMove, startLocation,1);
                    char endLocation[] = {halfMove->move[1],halfMove->move[2]};
                    setEndLocation(halfMove,endLocation);
                }
                break;
            }
            if (isupper(halfMove->move[0])){
                halfMove->movedPiece = halfMove->move[0];
                if(halfMove->move[1] == 'x'){
                    char endLocation[] = {halfMove->move[2],halfMove->move[3]};
                    setEndLocation(halfMove,endLocation);
                    halfMove->capture = 1;
                    break;
                } else if(halfMove->move[3] == '#'){
                    char endLocation[] = {halfMove->move[1],halfMove->move[2]};
                    setEndLocation(halfMove,endLocation);
                    halfMove->checkmate = 1;
                    break;
                }else if(halfMove->move[3] == '+'){
                    char endLocation[] = {halfMove->move[1],halfMove->move[2]};
                    setEndLocation(halfMove,endLocation);
                    halfMove->check = 1;
                    break;
                } else {
                    char startLoc[2] = {halfMove->move[1]};
                    setStartLocation(halfMove, startLoc,1);
                    char endLocation[] = {halfMove->move[2],halfMove->move[3]};
                    setEndLocation(halfMove,endLocation);
                    break;
                }
            }

            if (halfMove->move[0] == 'x' && halfMove->move[3] == '+'){
                halfMove->movedPiece = 'p';
                char endLocation[] = {halfMove->move[1],halfMove->move[2]};
                setEndLocation(halfMove,endLocation);
                halfMove->check = 1;
                halfMove->capture = 1;
                break;
            }else if (halfMove->move[0] == 'x'&& halfMove->move[3] == '#'){
                halfMove->movedPiece = 'p';
                char endLocation[] = {halfMove->move[1],halfMove->move[2]};
                setEndLocation(halfMove,endLocation);
                halfMove->checkmate = 1;
                halfMove->capture = 1;
                break;
            }else if (halfMove->move[0] == 'x'){

                halfMove->movedPiece = 'p';
                char startLocation[2] = {halfMove->move[1]};
                setStartLocation(halfMove, startLocation,1);
                char endLocation[] = {halfMove->move[2],halfMove->move[3]};
                setEndLocation(halfMove,endLocation);
                halfMove->capture = 1;
                break;
            } else if (halfMove->move[3] == '+'){
                if(isupper(halfMove->move[2])){
                    halfMove->movedPiece = halfMove->move[2];
                    char endLocation[] = {halfMove->move[0],halfMove->move[1]};
                    setEndLocation(halfMove,endLocation);
                } else {
                    halfMove->movedPiece = 'p';
                    char startLocation[2] = {halfMove->move[0]};
                    setStartLocation(halfMove, startLocation,1);
                    char endLocation[] = {halfMove->move[1],halfMove->move[2]};
                    setEndLocation(halfMove,endLocation);
                }
                halfMove->check = 1;
                break;
            } else if (halfMove->move[3] == '#'){
                if(isupper(halfMove->move[2])){
                    halfMove->movedPiece = halfMove->move[2];
                    char endLocation[] = {halfMove->move[0],halfMove->move[1]};
                    setEndLocation(halfMove,endLocation);
                } else {
                    halfMove->movedPiece = 'p';
                    char startLocation[2] = {halfMove->move[0]};
                    setStartLocation(halfMove, startLocation,1);
                    char endLocation[] = {halfMove->move[1],halfMove->move[2]};
                    setEndLocation(halfMove,endLocation);
                }
                halfMove->checkmate = 1;
                break;
            } else {
                halfMove->movedPiece = 'p';
                char startLocation[2] = {halfMove->move[0],halfMove->move[1]};
                setStartLocation(halfMove, startLocation,2);
                char endLocation[] = {halfMove->move[2],halfMove->move[3]};
                setEndLocation(halfMove,endLocation);
                break;
            }
            break;
        case 5:
            if (strcmp(halfMove->move, "O-O-O") == 0 || strcmp(halfMove->move, "0-0-0") == 0) {
                halfMove->queenSideCastling = 1;
                break;
            } else if (isupper(halfMove->move[0])){ // Case where a piece other than a pawn moves
                halfMove->movedPiece = halfMove->move[0];
                if(halfMove->move[1] == 'x'){
                    halfMove->capture = 1;
                    if (halfMove->move[4] == '+'){
                        halfMove->check = 1;
                        char endLocation[] = {halfMove->move[2],halfMove->move[3]};
                        setEndLocation(halfMove,endLocation);
                        break;
                    } else if(halfMove->move[4] == '#'){
                        halfMove->checkmate = 1;
                        char endLocation[] = {halfMove->move[2],halfMove->move[3]};
                        setEndLocation(halfMove,endLocation);
                        break;
                    }
                } else if (halfMove->move[2] == 'x') {
                    halfMove->movedPiece = halfMove->move[0];
                    char startLocation[] = {halfMove->move[1]};
                    setStartLocation(halfMove,startLocation,1);
                    char endLocation[] = {halfMove->move[3],halfMove->move[4]};
                    setEndLocation(halfMove,endLocation);
                    break;
                } else {
                        char startLocation[] = {halfMove->move[1],halfMove->move[2]};
                        setStartLocation(halfMove,startLocation,2);
                        char endLocation[] = {halfMove->move[3],halfMove->move[4]};
                        setEndLocation(halfMove,endLocation);
                        break;
                }    
            } else if (isupper(halfMove->move[4])){
                halfMove->pawnPromotion = 1;
                halfMove->movedPiece = halfMove->move[4];
                halfMove->capture = 1;
                char startLocation[] = {halfMove->move[0]};
                setStartLocation(halfMove,startLocation,1);
                char endLocation[] = {halfMove->move[2],halfMove->move[3]};
                setEndLocation(halfMove,endLocation);
                break;
            } else {
                halfMove->movedPiece = 'p';
                halfMove->capture = 1;
                char startLocation[] = {halfMove->move[0]};
                setStartLocation(halfMove,startLocation,1);
                char endLocation[] = {halfMove->move[2],halfMove->move[3]};
                setEndLocation(halfMove,endLocation);
                if(halfMove->move[4] == '+'){
                    halfMove->check = 1;
                } else if (halfMove->move[4] == '#'){
                    halfMove->checkmate = 1;
                }
                break;
            }
            break;
        case 6:
            if (isupper(halfMove->move[0])) {
                halfMove->movedPiece = halfMove->move[0];
                if(halfMove->move[4] == '+'){
                    char startLocation[] = {halfMove->move[1],halfMove->move[2]};
                    setStartLocation(halfMove,startLocation,2);
                    char endLocation[] = {halfMove->move[3],halfMove->move[4]};
                    setEndLocation(halfMove,endLocation);
                    halfMove->check = 1;
                    break;
                } else if (halfMove->move[4] == '#'){
                    char startLocation[] = {halfMove->move[1],halfMove->move[2]};
                    setStartLocation(halfMove,startLocation,2);
                    char endLocation[] = {halfMove->move[3],halfMove->move[4]};
                    setEndLocation(halfMove,endLocation);
                    halfMove->checkmate = 1;
                    break;
                } else {
                    char startLocation[] = {halfMove->move[1],halfMove->move[2]};
                    setStartLocation(halfMove,startLocation,2);
                    char endLocation[] = {halfMove->move[4],halfMove->move[5]};
                    setEndLocation(halfMove,endLocation);
                    halfMove->capture = 1;
                    break;
                }
            } else {
                halfMove->movedPiece = halfMove->move[4];
                halfMove->pawnPromotion = 1;
                if(halfMove->move[5] == '+'){
                    halfMove->check = 1;
                } else if (halfMove->move[5] == '#'){
                    halfMove->checkmate = 1;
                }
                char startLocation[] = {halfMove->move[0]};
                setStartLocation(halfMove,startLocation,1);
                char endLocation[] = {halfMove->move[2],halfMove->move[3]};
                setEndLocation(halfMove,endLocation);
                break;
            }
            break;
        case 7:
            halfMove->movedPiece = halfMove->move[0];
            char startLocation[] = {halfMove->move[1],halfMove->move[2]};
            setStartLocation(halfMove,startLocation,2);
            char endLocation[] = {halfMove->move[4],halfMove->move[5]};
            setEndLocation(halfMove,endLocation);
            halfMove->capture = 1;
            if(halfMove->move[6] == '+'){
                halfMove->check = 1;
                break;
            } else if (halfMove->move[6] == '#'){
                halfMove->checkmate = 1;
                break;
            }
            break;
        default:
            break;            
    }
    printf("Moove: %s , moved piece: %c , capture : %d, pawnPromotion: %d, check: %d, checkmate: %d", halfMove->move ,halfMove->movedPiece,halfMove->capture, halfMove->pawnPromotion, halfMove->check, halfMove->checkmate );
    printf("end location: %d %d , start location: %d %d \n", halfMove->endLocation[0],halfMove->endLocation[1], halfMove->startLocation[0], halfMove->startLocation[1] );
}

/*****************************************************************************/

/**
 * The following functions are the link between the SQL and the C code
*/
/*Datum chessboard_in(PG_FUNCTION_ARGS);
Datum chessboard_out(PG_FUNCTION_ARGS); // d'abord on déclare la fonction

PG_FUNCTION_INFO_V1(chessboard_in); // lien entre SQL et C
Datum chessboard_in(PG_FUNCTION ARGS){
    char *str = PG_GETARG_CSTRING(0); // on récupère le string entrant du coté SQL
    PG_RETURN_POINTER(fen_parse(str)); // on renvoie le pointeur vers le struct C
    // à noté que le coté SQL s'en fout du type de pointeur qu'on renvoie du moment qu'on lui indique que c'est un pointeur
}

PG_FUNCTION_INFO_V1(fen_out); // lien entre SQL et C
Datum fen_out(PG_FUNCTION_ARGS){ // Datum c'est la manière de définir un data type générique
    // ici on récupère le pointeur vers le SQL datatype du chessboard et on le cast en pointeur du struct C
    // en bref la ligne suivante fait une conversion SQL datatype => struct C
    ChessBoard *c = (ChessBoard *)PG_GETARG_POINTER(0); 
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
    ChessBoard *c = (ChessBoard *)PG_GETARG_POINTER(0);
    PG_RETURN_CSTRING(c->board);    
}

// Chessgame's Getters

Datum getFirstMoves(PG_FUNCTION_ARGS);

*/

/**
*   TESTS
*/
void test_getFirstMoves() {
    char* pgn = "1. e4 e5 2. Bc4 Nf6 3. Qf3 d6 4. d3 Nc6 5. b3 Nd4 6. Qe3 Nxc2+ 7. Ke2 Nxe3 8. Bxe3 Ng4 9. h3 Nxe3 10. fxe3 c5 11. Nf3 f6 12. Rf1 Qa5 13. Nbd2 Qc3 14. Rac1 Qb2 15. d4 Qxa2 16. dxc5 dxc5 17. Ra1 Qb2 18. Rfb1 Qc2 19. Ra3 b5 20. Bxb5+ Bd7 21.";
    char* expected = "1. e4 e5 2. Bc4 Nf6 3. Qf3";
    if(strcmp(getFirstMoves(pgn, 5), expected) == 0)
        printf("Test getFirstMoves passed\n");
    else    
        printf("Test getFirstMoves failed\n");
}



int main()
{
    test_getFirstMoves();





    /*// Test the FEN to Struct
    char str[] = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq e3 0 1";
    ChessBoard *c;
    c = fen_parse(str);
        printf("'%s'\n", c->fen);
        printf("'%s'\n", c->turn);
        printf("'%s'\n", c->castling);
        printf("'%s'\n", c->en_passant);
        printf("'%d'\n", c->halfmove_clock);
        printf("'%d'\n", c->fullmove_number);
   
    char *str2;
    str2 = chessboard_to_str(c);
    printf("'%s'\n", str2);

    // Test the parse of the moves
    const char *pgn = "1. e4 e5 2. Nf3 Nc6 3. Bb5 a6 4. Ba4 Nf6";
    
    ChessGame *game = chessMove_parse(pgn);
    
    printf("%s\n", getFirstMoves(game, 7));

    free(game);*/

    return 0;
}
