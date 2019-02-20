#include "../Move.h"
#include "../ArrayUtilities.h"
#include "./definitions.h"
#include<string.h>

void allocateMoveFields(Pos* p, Move* m) {
  m->coords = malloc(p->n * sizeof(int));
}

int moveEquals(Move* a, Move* b) {
  return ((a->n == b->n) && (memcmp(a->coords, b->coords, (a->n) * sizeof(int)) == 0));
}

Move* copyMove(Move* m) {
  Move* copy = malloc(sizeof(Move));
  copy->n = m->n;
  copy->coords = malloc(copy->n * sizeof(int));
  memcpy(copy->coords, m->coords, copy->n * sizeof(int));
  return copy;
}

void setMove(Move* set, Move* copy) {
  set->n = copy->n;
  memcpy(set->coords, copy->coords, set->n * sizeof(int));
}

void set2Move(Move* m, int x, int y) {
  m->n = 2;
  m->coords[0] = x;
  m->coords[1] = y;
}

Move* getMoves(Pos* p, int* numMoves) {
  *numMoves = 0;
  for(int i = 0; i < p->numRows; i++) {
    *numMoves += p->rows[i];
  }
  Move* moves = malloc(*numMoves * sizeof(Move));
  getMovesHelper(p, p->rows, p->n, moves);
  return moves;
}

void getMovesHelper(Pos* p, int* rows, int n, Move* moves) {
  if(n == 1){
    for(int x = 0; x < rows[0]; x++) {
      moves[x].n = 1;
      moves[x].coords = malloc(MIN_ALLOC * sizeof(int));
      moves[x].coords[0] = x;
    }
    return;
  }
  //don't technically need this, as n == 1 is the base case
  //but it's probably faster to have this
  if(n == 2) {
    int move = 0;
    int height = p->dims[1];
    for (int y = 0; y < height; y++) {
      int row = rows[y];
      for(int x = 0; x < row; x++) {
        moves[move].n = 2;
        moves[move].coords = malloc(MIN_ALLOC * sizeof(int));
        moves[move].coords[0] = x;
        moves[move].coords[1] = y;
        move += 1;
      }
    }
    return;
  }

  int moveBy = factorial(p->dims, 1, n-2);
  for(int abstractLevel = 0; abstractLevel < p->dims[n-1]; abstractLevel++) {
    int movesAtLevel = 0; //num moves at this level
    for(int i = 0; i < moveBy; i++) {
      movesAtLevel += rows[abstractLevel * moveBy + i];
    }
    getMovesHelper(p, rows + (abstractLevel * moveBy), n-1, moves);
    moves += movesAtLevel;
    for(int x = 1; x <= movesAtLevel; x++) {
      moves[-1 * x].n = n;
      moves[-1 * x].coords[n-1] = abstractLevel;
    }
  }
  return;
}

void freeMove(Move* m) {
  free(m->coords);
  free(m);
}

void freeMovesList(Move* mList, int size) {
  for(int i = 0; i < size; i++) {
    free(mList[i].coords);
  }
  free(mList);
}

void printMove(Move* m) {
  printArray(m->n, m->coords, stdout);
  printf("\n");
  fflush(stdout);
}

void printAllMoves(Pos* p) {
  if(!p->rows) {
    printf("There are no possible moves because the game is over.\n");
    return;
  }
  int numMoves;
  Move* moves = getMoves(p, &numMoves);
  printf("There are %d moves\n", numMoves);
  for(int i = 0; i < numMoves; i++) {
    printArray(moves[i].n,moves[i].coords, stdout);
    printf("\n");
  }
  freeMovesList(moves, numMoves);
}
