#ifndef GAME_SPECIFIC_HUMAN_INTERACTION_H
#define GAME_SPECIFIC_HUMAN_INTERACTION_H

#include "./Move.h"
#include "./Pos.h"

Move* getHumanMove(int n);
Move* getValidHumanMove(Pos* p);

#endif
