//#include <postgres.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>


/*****************************************************************************/

/* Structure to represent the chessgame */

typedef struct
{
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

typedef struct
{
    char      pgn[2000];
    ChessHalfMove halfMoves[];

} ChessGame;

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

int main(){

    return 0;
    //Origine de l’erreur, je passe en argument a chaque fois de mes variables, les pointeurs donc quand le string est modifié, toutes les donées qui utilisaient ce pointeur se font corrompre->
}