#ifndef MECHANICALTURK_REDEFINES_H
#define MECHANICALTURK_REDEFINES_H

#include "Game-redefines.h"
#define action Action
#define Game unused_Game
#include "mechanicalTurk.h"
#undef action
#undef Game

#endif
