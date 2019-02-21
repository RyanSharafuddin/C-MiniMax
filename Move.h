#ifndef MOVE_H
#define MOVE_H
#include<stdlib.h>
#include<stdio.h>

//forward declarations
struct Pos;
typedef struct Move Move;

//when done, free the moves array as well as the coords of each move
//This function returns a pointer to an array with all possible moves
//and stores the number of moves at the address pointed to by numMoves
Move* getMoves(struct Pos* p, int* numMoves);
Move* copyMove(Move* m);
int moveEquals(Move* a, Move* b);
//Given an already allocated pointer to a Move, allocate its fields
void allocateMoveFields(struct Pos* p, Move* m);
//does not deallocate m, just its fields
void deallocateMoveFields(Move* m);
/* Given a Move* set that already has enough space allocated (including for
 * coords), this function copies the stuff from copy over to set.
 */
void setMove(Move* set, Move* copy);
/* Specifically for use in Bot.c's apply2DOptimization.
 */
void set2Move(Move* m, int x, int y);

//debugging purposes
void printAllMoves(struct Pos* p);
void printMove(Move* m);


//freeing stuff
void freeMove(Move* m);
void freeMovesList(Move* m, int size);



#endif /* MOVE_H */
