#ifndef MOVE_H
#define MOVE_H
#include<stdlib.h>
#include<stdio.h>
#include"Pos.h"

#define MIN_ALLOC 7
typedef struct Move {
  int* coords;
  int n; //number of dimensions
} Move;

//when done, free the moves array as well as the coords of each move
//This function returns a pointer to an array with all possible moves
//and stores the number of moves at the address pointed to by numMoves
Move* getMoves(Pos* p, int* numMoves);
Move* copyMove(Move* m);
int moveEquals(Move* a, Move* b);
/* Given a Move* set that already has enough space allocated (including for
 * coords), this function copies the stuff from copy over to set.
 */
void setMove(Move* set, Move* copy);
/* Specifically for use in Bot.c's apply2DOptimization.
 */
void set2Move(Move* m, int x, int y);

//debugging purposes
void printAllMoves(Pos* p);
void printMove(Move* m);

//internal utility functions
void getMovesHelper(Pos* p, int* rows, int n, Move* moves);

//freeing stuff
void freeMove(Move* m);
void freeMovesList(Move* m, int size);



#endif /* MOVE_H */
