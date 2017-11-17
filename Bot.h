#ifndef BOT_H
#define BOT_H
#include<string.h>
#include"ArrayUtilities.h"
#include"Pos.h"
#include"Move.h"

#include "myHash.h"
#include "Hashtable_Stuff/hashtable.h"
#include "Hashtable_Stuff/hashtable_itr.h"

typedef struct MovPos {
  Move* m;
  Pos* p;
} MovPos;

typedef struct hashtable hashtable;

MovPos newMovPos(Move* move, Pos* position);
void freeUnknowns(MovPos** unknowns, size_t n);

//Store the bestmove in m
int miniMax(Pos* p, int computer, int depth, Move* m);
/* Returns 1 if the value of this pos is known
 * Whether m is NULL or not signifies if we want it to put the winning move into
 * the space pointed to by m.
 * If the pos value is known, we put whether or not it's a winning position into
 * the address pointed to by winning
 */
int knownPos(Pos* p, int* winning, Move* m);
void addLosingPos(Pos* losingPos);
void addWinningPos(Pos* winningPos, Move* winningMove);
/*
 * Returns true if can apply a 2D optimization
 * and puts winning or not into winning
 * and changes move to point at winning move if winning
 */
int apply2DOptimization(Pos* p, int* winning, Move* move);
int isFullHyperRect(Pos* p);







#endif
