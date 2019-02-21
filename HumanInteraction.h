#ifndef HUMAN_INTERACTION_H
#define HUMAN_INTERACTION_H
#include "Move.h"
#include "Pos.h"

int getInt();
int getYesNo(char* prompt);
//given int n, returns array of that many ints
int* getNTuple(int n, char* prompt);

//Puts number of dims into address pointed to by n and returns pointer to dim array;
int* getDims(int* n);

#endif
