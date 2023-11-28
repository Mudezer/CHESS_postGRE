/*
 * chess.c
 */

#include <postgres.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <float.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

#include "utils/builtins.h"
#include "libpq/pqformat.h"

#include "chess.h"

PG_MODULE_MAGIC;

/*****************************************************************************/

/*****************************************************************************
 * Make
 *****************************************************************************/

static ChessGame * chessgame_make(const char * pgn)
{
    ChessGame *c = malloc(sizeof(ChessGame)); // TODO replace by palloc 
    c->pgn = pgn;
    SCL_recordFromPGN(c->record, pgn);
    return c;
}


static ChessBoard * chessboard_make(const char fen[MAX_FEN_LENGTH])
{
    ChessBoard *c = malloc(sizeof(ChessBoard)); // TODO replace by palloc 
    SCL_boardFromFEN(c->board, fen);
    strcpy(c->fen, fen);
    return c;
}



/*****************************************************************************
 * Parser & verification of errors TODO
 *****************************************************************************/

/**
 * Parse an input that has a FEN format to create a ChessBoard struct
 * @param fen : the FEN format input
 * @return a pointer to the ChessBoard struct
*/
static ChessBoard * chessboard_parse(const char fen[MAX_FEN_LENGTH])
{
  /* // Test if the make is well done ?
  char * a, b, c, d, e, f;
  if (sscanf(fen, "%s %s %s %s %s %s", &a, &b, &c, &d, &e, &f) != 6)
    ereport(ERROR,(errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
      errmsg("invalid input syntax for type %s: \"%s\"", "complex", str)));
    */
  return chessboard_make(fen);
}


/**
 * Parse an input that has a PGN format to create a ChessGame struct
 * @param pgn : the PGN format input
 * @return a pointer to the ChessGame struct
*/
static ChessGame * chessgame_parse(const char * pgn)
{
  /* // Test if the make is well done ?
  char * a;
  if (sscanf(fen, "%s", &a) != 1)
    ereport(ERROR,(errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
      errmsg("invalid input syntax for type %s: \"%s\"", "complex", str)));
    */
  return chessgame_make(pgn);
}


/*****************************************************************************
 * Cast functions
 *****************************************************************************/
/**
 * Getter of the FEN format of a ChessBoard
 * @param fen : the ChessBoard struct
 * @return the FEN format of the ChessBoard
*/
static char * chessboard_to_str(const ChessBoard *c)
{
  return c->fen;
}


/**
 * Getter of the PGN format of a ChessGame
 * @param fen : the ChessGame struct
 * @return the PGN format of the ChessGame
*/
static char * chessgame_to_str(const ChessGame *c)
{
  return c->pgn;
}



/*****************************************************************************
 * C-SQL Linking Functions
 *****************************************************************************/

/**
 * The following functions are the link between the SQL and the C code
*/

/*********************************IN/OUT**************************************/


PG_FUNCTION_INFO_V1(chessboard_in); // Declare the function
Datum chessboard_in(PG_FUNCTION_ARGS)
{
  char *fen = PG_GETARG_CSTRING(0); // String input from the SQL
  PG_RETURN_ChessBoard_P(chessboard_parse(fen)); // Send the pointer to the C struct
  // à noter que le coté SQL s'en fout du type de pointeur qu'on renvoie du moment qu'on lui indique que c'est un pointeur
}


PG_FUNCTION_INFO_V1(chessboard_out);
Datum chessboard_out(PG_FUNCTION_ARGS) // Datum to declare a generic data type
{
  ChessBoard *c = PG_GETARG_ChessBoard_P(0); // Pointer to the SQL datatype & cast it in struct C
  char *fen = chessboard_to_str(c); // To only have the FEN 
  PG_FREE_IF_COPY(c, 0);
  PG_RETURN_CSTRING(fen);
}



PG_FUNCTION_INFO_V1(chessgame_in); // Declare the function
Datum chessgame_in(PG_FUNCTION_ARGS)
{
  char *pgn = PG_GETARG_CSTRING(0); // String input from the SQL
  PG_RETURN_ChessGame_P(chessgame_parse(pgn)); // Send the pointer to the C struct
  // à noter que le coté SQL s'en fout du type de pointeur qu'on renvoie du moment qu'on lui indique que c'est un pointeur
}


PG_FUNCTION_INFO_V1(chessgame_out);
Datum chessgame_out(PG_FUNCTION_ARGS) // Datum to declare a generic data type
{
  ChessGame *c = PG_GETARG_ChessGame_P(0); // Pointer to the SQL datatype & cast it in struct C
  char *pgn = chessgame_to_str(c); // To only have the PGN 
  PG_FREE_IF_COPY(c, 0);
  PG_RETURN_CSTRING(pgn);
}



/************************************CAST*************************************/

PG_FUNCTION_INFO_V1(chessboard_cast_from_text);
Datum chessboard_cast_from_text(PG_FUNCTION_ARGS)
{
  text *txt = PG_GETARG_TEXT_P(0);
  char *fen = DatumGetCString(DirectFunctionCall1(textout, PointerGetDatum(txt)));
  PG_RETURN_ChessBoard_P(chessboard_parse(fen));
}


PG_FUNCTION_INFO_V1(chessboard_cast_to_text);
Datum chessboard_cast_to_text(PG_FUNCTION_ARGS)
{
  ChessBoard *c  = PG_GETARG_ChessBoard_P(0);
  text *out = (text *)DirectFunctionCall1(textin, PointerGetDatum(chessboard_to_str(c)));
  PG_FREE_IF_COPY(c, 0);
  PG_RETURN_TEXT_P(out);
}



PG_FUNCTION_INFO_V1(chessgame_cast_from_text);
Datum chessgame_cast_from_text(PG_FUNCTION_ARGS)
{
  text *txt = PG_GETARG_TEXT_P(0);
  char *pgn = DatumGetCString(DirectFunctionCall1(textout, PointerGetDatum(txt)));
  PG_RETURN_ChessGame_P(chessgame_parse(pgn));
}


PG_FUNCTION_INFO_V1(chessgame_cast_to_text);
Datum chessgame_cast_to_text(PG_FUNCTION_ARGS)
{
  ChessGame *c  = PG_GETARG_ChessGame_P(0);
  text *out = (text *)DirectFunctionCall1(textin, PointerGetDatum(chessgame_to_str(c)));
  PG_FREE_IF_COPY(c, 0);
  PG_RETURN_TEXT_P(out);
}



/*********************************CONTRUCTOR**********************************/


/*
PG_FUNCTION_INFO_V1(chessboard_constructor);
Datum chessboard_constructor(PG_FUNCTION_ARGS)
{
  char * fen = PG_GETARG_CSTRING(0);
  PG_RETURN_ChessBoard_P(chessboard_make(fen));
}



PG_FUNCTION_INFO_V1(chessgame_constructor);
Datum chessgame_constructor(PG_FUNCTION_ARGS)
{
  char * pgn = PG_GETARG_CSTRING(0);
  PG_RETURN_ChessGame_P(chessgame_make(pgn));
}
*/



/*****************************************************************************
 * Functions to implement
 *****************************************************************************/

/*****TODO Move to chess.h ?************/
typedef struct
{
    char        firstHalfMove[MAX_MOVE_LENGTH];
    char        secondHalfMove[MAX_MOVE_LENGTH];
    int         moveNumber;
} ChessMove;


typedef struct{
    char    move[10];
    int     halfMoveNumber;
    char    movedPiece;
    int     endLocation[2];
    int     startLocation[2];
    int     capture ;
    int     kingSideCastling;
    int     queenSideCastling;
    int     check;
    int     pawnPromotion;
    int     checkmate;
} ChessHalfMove;


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
 * @param gamePGN the game to analyze
 * @param n the number of moves to print
 * @returns  n first moves of a ChessGame in PGN format
*/
static char * getFirstMoves(char * gamePGN, int n)
{
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
    return openingPGN;
}



/**
 * @param gamePGN the game to analyze
 * @param opening the opening to find
 * @returns 1 if the game has the opening, 0 otherwise
*/
static int hasOpening(char* gamePGN, char* openingPGN)
{
    int isSame = 0;
    if(strstr(gamePGN, openingPGN) != NULL)
        isSame = 1;
    return isSame;
}


/**
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
    char board_tofindStr[65]; // Increase the size by 1 to accommodate the null terminator
    strncpy(board_tofindStr, *board_tofind, 64);
    board_tofindStr[64] = '\0'; // Add the null terminator

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
        strncpy(boardStr, *board, 64);
        boardStr[64] = '\0';
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
static char* getBoard(char* gamePGN, int moveNumber)
{
    char* boardFEN = malloc(sizeof(char)*MAX_FEN_LENGTH);

    SCL_Record *record = malloc(SCL_RECORD_MAX_SIZE);
    SCL_recordFromPGN(*record, gamePGN);
    SCL_Board *board = malloc(SCL_RECORD_MAX_SIZE);
    SCL_recordApply(*record, *board, moveNumber);
    SCL_boardToFEN(*board, boardFEN);

    free(record);
    free(board);

    return boardFEN;
}




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


int main()
{




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
