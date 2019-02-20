#ifndef POS_H
#define POS_H
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include "ArrayUtilities.h"
#include"myHash.h"
#include "Hashtable_Stuff/hashtable.h"
#include "Hashtable_Stuff/hashtable_itr.h"

//forward declarations
struct Move;
typedef struct Pos Pos;
struct Pos {
  int n;      //the n in n-dimensional pos. The length of dims
  int* dims;  //array of dimension measurements
  int* rows;  //1D array of rows
  int numRows;  //all space taken up by all dimensions
};


Pos* newPos(int* dims, size_t n); //should free pos pointer and rows pointer when done
Pos* copyPos(Pos* p);             //copy constructor
void printPos(Pos* pos, int pretty, FILE* fp);
void makeMove(Pos** p, struct Move* m);
int equalPositions(Pos* a, Pos* b);
unsigned int hashPosition(Pos* p);


//freeing stuff
//IMPORTANT: MAKE DECISION ON WHETHER OR NOT TO FREE DIMS BASED ON WHETHER COPY DIMS OR NOT
//WHICH IN TURN IS BASED ON HOW YOU TIP OVER (OR NOT)
//DO FREE DIMS (and copy in copy constructor)
//Frees *p and makes it point to null
void freePos(Pos** p);

#endif /* POS_H */
