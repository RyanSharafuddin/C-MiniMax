#include "Pos.h"
#include "Move.h"

Pos* newPos(int* dims, size_t n) {
  Pos* p = malloc(sizeof(Pos));
  if(!p){
    return NULL;
  }
  p->n = n;
  p->dims = dims;
  p->numRows = factorial(dims, 1, n-1);
  p->rows = malloc(p->numRows * sizeof(int));
  if(!p->rows) {
    return NULL;
  }
  for(int i = 0; i < p->numRows; i++) {
    p->rows[i] = dims[0];
  }
  return p;
}

Pos* copyPos(Pos* p) {
  Pos* copy = malloc(sizeof(Pos));
  copy->n = p->n;
  copy->numRows = p->numRows;
  copy->dims = malloc(p->n * sizeof(int));
  memcpy(copy->dims, p->dims, (copy->n * sizeof(int)));
  copy->rows = malloc(copy->numRows * sizeof(int));
  memcpy(copy->rows, p->rows, p->numRows * sizeof(int));
  return copy;
}

void freePos(Pos** p) {
  free((*p)->dims);
  free((*p)->rows);
  free(*p);
  *p = NULL;
}

int equalPositions(Pos* a, Pos* b) {
  //debug - numRows should not be needed
  if((a->n == b->n) && (memcmp(a->dims, b->dims, a->n * sizeof(int)) == 0) && (a->numRows != b->numRows)) {
    printf("NUMROWS ERROR ERROR ERROR\n");
    exit(1);
  }
  return( (a->n == b->n) &&
          (memcmp(a->dims, b->dims, a->n * sizeof(int)) == 0) &&
          (memcmp(a->rows, b->rows, a->numRows * sizeof(int)) == 0) );
}

unsigned int hashPosition(Pos* p) {
  size_t s = p->numRows * sizeof(int);
  return SuperFastHash((char*) p->rows, s);
}

void makeMove(Pos** p, Move* m) {
  if(sum(m->coords, 0, m->n - 1) == 0) {
    printf("Game over\n\n");
    freePos(p);
    return;
  }
  //don't call shorten if already 1 dimensional
  if((*p)->n == 1) {
    (*p)->rows[0] = m->coords[0];
    (*p)->dims[0] = m->coords[0];
    return;
  }
  //can guarantee there's at least 1 nonzero value because of the sum thing above.
  int index;
  int value;
  if(oneNonZero(m->coords, &index, &value, m->n)) {
    shorten(*p, index, value);
    return;
  }
  int leadingZeros = lenBeforeNonZero(m->coords, 0, m->n-1);
  makeMoveHelper((*p)->dims, (*p)->n, (*p)->rows, leadingZeros, m->coords);
  return;
}

void shorten(Pos* p, int index, int shortenedVal) {
  if(index == 0) {
    if(shortenedVal == 1) {
      int rowsPerSquare = p->dims[1];
      int numSquares = factorial(p->dims, 2, p->n-1);
      for(int square = 0; square < numSquares; square++) {
        p->rows[square] = lenBeforeZero(p->rows + (square * rowsPerSquare), 0, p->dims[1] -1);
      }
      memmove(p->dims, p->dims + 1, (p->n - 1) * sizeof(int));
      p->numRows = numSquares;
      p->n -= 1;
      return;
    }
    for(int row = 0; row < p->numRows; row++) {
      p->rows[row] = (shortenedVal < p->rows[row]) ? shortenedVal : p->rows[row];
    }
    p->dims[0] = shortenedVal;
    return;
  }
  //move each thing over, then change DIMS, then change numRows, then change n
  //valid b/c factorial will return 1 if start > end
  int layersToShorten = factorial(p->dims, index+1, p->n-1);
  //size of layer in rows
  int currentLayerSize = factorial(p->dims, 1, index);
  int newLayerSize = factorial(p->dims, 1, index-1) * shortenedVal;
  for(int layer = 1; layer < layersToShorten; layer++) { //can start at 1, since no need to copy zeroeth thing to where it already is
    int* copyFrom = p->rows + (layer * currentLayerSize);
    int* copyTo = p->rows + (layer * newLayerSize);
    memmove(copyTo, copyFrom, newLayerSize * sizeof(int));
  }
  p->numRows -= layersToShorten * (currentLayerSize - newLayerSize);
  if(shortenedVal == 1) {
    memmove(p->dims + index, p->dims + index + 1, (p->n - 1 - index) * sizeof(int));
    p->n -= 1;
  }
  else {
    p->dims[index] = shortenedVal;
  }
  return;
}

void makeMoveHelper(int* dims, int n, int* rows, int zeros, int* moveCoords) {
  if(n == 2) {
    for(int height = moveCoords[1]; height < dims[1]; height++) {
      rows[height] = (moveCoords[0] < rows[height]) ? moveCoords[0] : rows[height];
    }
    return;
  }

  int moveBy = factorial(dims, 1, n-2);
  for(int abstractLevel = moveCoords[n-1]; abstractLevel < dims[n-1]; abstractLevel++) {
    //if the abstractLevel is already zero, return
    if(rows[abstractLevel * moveBy] == 0) {
      return;
    }
    //if all lower moveCoords are zero
    //then memset this and all higher abstract Levels and return
    if(zeros == n-1) {
      memset(rows + abstractLevel * moveBy, 0, moveBy * (dims[n-1] - abstractLevel) * sizeof(int));
      return;
    }
    makeMoveHelper(dims, n-1, rows + (abstractLevel * moveBy), zeros, moveCoords);
  }
}

void printPos(Pos* p, int pretty, FILE* fp) {
  printPosHelper(p, pretty, p->n, p->rows, 1, fp);
}
void printPosHelper(Pos* p, int pretty, int n, int* start, int callNum, FILE* fp) {
  if(!p) {
    printf("\nThe pos is empty!\n");
  }
  if(!pretty || n > 4) {
    fprintf(fp, "%d ", p->n);
    printArray(n, p->dims, fp);
    fprintf(fp, " ");
    fprintf(fp, "%d ", p->numRows);
    fprintf(fp, " ");
    printArray(p->numRows, p->rows, fp);
    return;
  }
  char* spaceBetweenSquares = "  ";
  if(n == 1) {
    printRow((p -> rows)[0], (p -> rows)[0], fp);
    if(callNum == 1) {
      fprintf(fp, "\n");
    }
    return;
  }
  if(n == 2) {
    int fullRow = p-> rows[0];
    for(int y = p -> dims[1]-1; y > -1; y--){
      printRow(p -> rows[y], fullRow, fp);
      fprintf(fp, "\n");
    }
  return;
  }
  if(n == 3) {
    int numRects = p->dims[2];
    int heightRect = p->dims[1];
    int fullRow = p->dims[0];
    for(int y = heightRect - 1; y > -1; y--) {
      for(int rect = 0; rect < numRects; rect++) {
        int rowWidth = start[rect*heightRect + y];
        printRow(rowWidth, fullRow, fp);
        fprintf(fp, "%s", spaceBetweenSquares);
      }
      fprintf(fp, "\n");
    }
  }
  if(n == 4) {
    int numCubes = p->dims[3];
    int squaresPerCube = p->dims[2];
    int heightSquare = p->dims[1];
    for(int cube = numCubes - 1; cube > -1; cube--) {
      printPosHelper(p, pretty, n-1, &p->rows[heightSquare * squaresPerCube * cube], callNum, fp);
      fprintf(fp, "\n");
    }
  }
}

//NO newline
void printRow(int rowSize, int fullSize, FILE* fp) {
  for(int x = 0; x < rowSize; x++) {
    fprintf(fp, "* ");
  }
  for(int x = 0; x < fullSize - rowSize; x++) {
    fprintf(fp, "  ");
  }
  return;
}
