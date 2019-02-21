#include "../GameSpecificHumanInteraction.h"
#include "./definitions.h"
#define MAX_DIGITS 20


//n is number of dimensions
Move* getHumanMove(int n) {
  Move* m = malloc(sizeof(Move));
  m->n = n;
  m->coords = getNTuple(n, "Enter coordinate");
  return m;
}

Move* getValidHumanMove(Pos* p) {
  int madeValid = 0;
  Move* hm;
  int numMoves;
  Move* mList = getMoves(p, &numMoves);
  while(!madeValid) {
    hm = getHumanMove(p->n);
    for(int i = 0; i < numMoves; i++) {
      if(moveEquals(hm, &mList[i])) {
        madeValid = 1;
      }
    }
    if(!madeValid) {
      printf("That's not a valid move, you buffoon!\n");
      freeMove(hm);
    }
  }
  freeMovesList(mList, numMoves);
  return hm;
}

Pos* humanInputPos() {
  int n;
  int* dims = getDims(&n);
  return newPos(dims, n);
}
