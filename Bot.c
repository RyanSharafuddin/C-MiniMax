#include "Bot.h"


extern hashtable* losingPositions;
extern hashtable* winningPositions;
extern int in;

MovPos newMovPos(Move* move, Pos* position) {
  MovPos mp;
  mp.m = move;
  mp.p = position;
  return mp;
}

void freeUnknowns(MovPos** unknowns, size_t n) {
  for(int i = 0; i < n; i++) {
    freePos(&((*unknowns)[i].p));
  }
  free(*unknowns);
  *unknowns = NULL;
}

int isFullHyperRect(Pos* p) {
  int base = p->rows[0];
  if(base == 1) { //so it does not mistakenly conclude single poison square is winning
    return 0;
  }
  for(int i = 1; i < p->numRows; i++) {
    if(p->rows[i] != base) {
      return 0;
    }
  }
  return 1;
}

void addLosingPos(Pos* p) {
  //you'll have to copy the pos, since this one will be changed.
  Pos* copy = copyPos(p);
  if(hashtable_insert(losingPositions, copy, &in)) {
    return;
  }
  else {
    printf("Error inserting into hashtable losingPositions\n");
    exit(1);
  }
}

void addWinningPos(Pos* winningPos, Move* winningMove) {
  //remember that the winning Move will be freed, so need to make copy here, not
  //just put in pointer
  //also note that you'll have to copy over the pos, since this one will be changed.
  Pos* copy = copyPos(winningPos);
  Move* w = copyMove(winningMove);
  if(hashtable_insert(winningPositions, copy, w)) {
    return;
  }
  else {
    printf("Error inserting into hashtable winningPositions\n");
    exit(1);
  }
}

int apply2DOptimization(Pos* p, int* winning, Move* move) {
  if(p->n == 1) {
    if(p->rows[0] == 1) {
      *winning = 0;
      return 1;
    }
    *winning = 1;
    if(move) {
      move->n = 1;
      move->coords[0] = 1;
    }
    return 1;
  }
  if(p->n != 2) {
    return 0;
  }
  int width = p->dims[0];
  int height = p->dims[1];
  if(width == height) {
    if(p->rows[1] >= 2) { //square
      *winning = 1;
      if(move) {
        set2Move(move, 1, 1);
      }
      return 1;
    }
    else { //Lshape loss
      *winning = 0;
      return 1;
    }
  }
  else if(p->rows[1] == 1) { //Lshape win
    *winning = 1;
    if(move) {
      int x = (width > height) ? height : 0;
      int y = (width > height) ? 0 : width;
      set2Move(move, x, y);
    }
    return 1;
  }
  if(width == 2) {
    int firstCol = height;
    int secondCol = contiguousCountOf(p->rows, height, 2);
    if(firstCol == secondCol + 1) {
      *winning = 0;
      return 1;
    }
    else {
      *winning = 1;
      if(move) {
        int x = (firstCol > secondCol) ? 0 : 1;
        int y = (firstCol > secondCol) ? secondCol + 1 : secondCol - 1;
        set2Move(move, x, y);
      }
      return 1;
    }
  }
  if(height == 2) {
    int top = p->rows[1];
    int bottom = p->rows[0];
    if(bottom == top + 1) {
      *winning = 0;
      return 1;
    }
    else {
      *winning = 1;
      if(move) {
        int x = (bottom > top) ? top + 1 : top - 1;
        int y = (bottom > top) ? 0 : 1;
        set2Move(move, x, y);
      }
      return 1;
    }
  }
  return 0;
}
/* Returns 1 if it's a pos whose value is known.
 * if wantWinningMove, puts winningMove in *m
 * if known value, puts value in *winning
 */
int knownPos(Pos* p, int* winning, Move* m) {
  if(!p || (!m && isFullHyperRect(p))) {
    *winning = 1;
    return 1;
  }
  if(apply2DOptimization(p, winning, m)) {
    return 1;
  }
  if (hashtable_search(losingPositions, p) != NULL) {
    *winning = 0;
    return 1;
  }
  Move* winningMove = hashtable_search(winningPositions, p);
  if(winningMove) {
    if(m) {
      m->n = winningMove->n;
      memcpy(m->coords, winningMove->coords, m->n * sizeof(int));
    }
    *winning = 1;
    return 1;
  }
  return 0;
}


int miniMax(Pos* p, int computer, int depth, Move* m) {

//DEBUG //////////////////////////
  // printf("Depth: %d\n", depth);
  // printf("Pos considering:\n");
  // printPos(p, 1, stdout);
/////////////////////////////////
  int winning;
  int known = knownPos(p, &winning, m);
  if(known && winning) {
    //DEBUG
    // printf("This is a known winning position. Return\n");
    return (computer) ? 1 : -1;
  }
 
  if(known && !winning) {
    //DEBUG
    // printf("This is a known losing position. Return\n");
    if(depth == 1) { //only getMoves and set losing move if you have to.
      //can optimize even further by making a get minimal move option,
      //so can set losing move without having to compute all available moves.
      int numMoves;
      Move* mList = getMoves(p, &numMoves);
      setMove(m, &mList[numMoves-1]);
      freeMovesList(mList, numMoves);
    }
    return (computer) ? -1 : 1;
  }

  int numMoves;
  Move* mList = getMoves(p, &numMoves);

  MovPos* unknowns = malloc(numMoves * sizeof(MovPos));
  int numUnknown = 0;

  for(int ithMove = 1; ithMove < numMoves; ithMove++) {
    Pos* copy = copyPos(p);
    makeMove(&copy, &mList[ithMove]);
    int ithPosWinning;
    int ithPosKnown = knownPos(copy, &ithPosWinning, NULL);

/////DEBUG///////////
    // printf("In moves loop for depth %d and pos\n", depth);
    // printPos(p, 1, stdout);
    // printf("Considering moveNum: %d\n", ithMove);
    // printMove(&mList[ithMove]);
    // printf("That move leads to the following pos\n");
    // printPos(copy, 1, stdout);
//DEBUG////////////////////////////

    if(ithPosKnown && !ithPosWinning) {
      //DEBUG
      // printf("Have found a move that forces a known losing position. Return\n");
      setMove(m, &mList[ithMove]);
      addWinningPos(p, &mList[ithMove]);
      freePos(&copy);
      freeUnknowns(&unknowns, numUnknown);
      freeMovesList(mList, numMoves);
      return (computer) ? 1 : -1;
    }

    if(ithPosKnown && ithPosWinning) {
      //DEBUG
      // printf("That move leads to a winning position. Moving on\n");
      freePos(&copy);
      continue;
    }

    //DEBUG
    // printf("This is an unknown position. Set aside for later\n");
    unknowns[numUnknown] = newMovPos(&mList[ithMove], copy);
    numUnknown += 1;
  }

  Move xWinner;
  xWinner.coords = malloc(p->n * sizeof(int));
  for(int xthUnknown = 0; xthUnknown < numUnknown; xthUnknown++) {
    //DEBUG////////
    // printf("Determining unknowns for pos\n");
    // printPos(p, 1, stdout);
    // printf("On unknown # %d\n", xthUnknown);
    /////////////////////
    int score = miniMax(unknowns[xthUnknown].p, !computer, depth+1, &xWinner);
    if((computer && score == 1) || (!computer && score == -1)) {
      //DEBUG////////
      // printf("Can force a losing position. Ending search\n");
      /////////////////////////
      setMove(m, unknowns[xthUnknown].m);
      addWinningPos(p, unknowns[xthUnknown].m);
      addLosingPos(unknowns[xthUnknown].p);
      freeUnknowns(&unknowns, numUnknown);
      freeMovesList(mList, numMoves);
      free(xWinner.coords);
      return (computer) ? 1 : -1;
    }
    else {
      //unknowns[xthUnknown].p is a winning position, and xWinner is what to do
      //DEBUG////////
      // printf("That unknown is a winning position. Adding to winningPositions\n");
      /////////////////////////
      addWinningPos(unknowns[xthUnknown].p, &xWinner);
    }
  }
  //DEBUG////////
  // printf("Welp, this position is a losing one. Adding to losing positions\n");
  /////////////////////////
  free(xWinner.coords);
  addLosingPos(p);
  freeUnknowns(&unknowns, numUnknown);
  setMove(m, &mList[numMoves-1]); //only necessary if depth is 1. Does checking decrease code speed?
  freeMovesList(mList, numMoves);
  return (computer) ? -1 : 1;
}
