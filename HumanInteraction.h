#ifndef HUMAN_INTERACTION_H
#define HUMAN_INTERACTION_H
#include "Move.h"
#include "Pos.h"

// Move* getHumanMove(int n);       //Delete
// Move* getValidHumanMove(Pos* p); //Delete
int getYesNo(char* prompt);

//Puts number of dims into address pointed to by n;
int* getDims(int* n);

#endif
