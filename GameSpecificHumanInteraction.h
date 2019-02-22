#ifndef GAME_SPECIFIC_HUMAN_INTERACTION_H
#define GAME_SPECIFIC_HUMAN_INTERACTION_H

#include "./Move.h"
#include "./Pos.h"
#include "./HumanInteraction.h"

Move* getHumanMove(int n); //TODO: delete this (not needed here, since it's an internal utility function for the corresponging C file)
Move* getValidHumanMove(Pos* p);

Pos* humanInputPos();

#endif
