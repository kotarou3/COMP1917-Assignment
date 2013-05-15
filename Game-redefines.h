#ifndef GAME_REDEFINES_H
#define GAME_REDEFINES_H

// Game.h wrapper to redefine the crap type names and to add some of our own

// Give better names to the structs and typedefs
#define region RegionLocation
#define arc EdgeLocation
#define vertex VertexLocation
#define degree DegreeType
#define uni PlayerId
#define action Action

// Ignore a conflicting typedef
#define _game _Game
#define Game unused_Game

#include "Game.h"

#undef region
#undef arc
#undef vertex
#undef degree
#undef uni
#undef action
#undef _game
#undef Game

#define NUM_PLAYERS NUM_UNIS

#define DEGREE_THD STUDENT_THD
#define DEGREE_BPS STUDENT_BPS
#define DEGREE_BQN STUDENT_BQN
#define DEGREE_MJ STUDENT_MJ
#define DEGREE_MTV STUDENT_MTV
#define DEGREE_MMONEY STUDENT_MMONEY

#define false 0
#define true !0

typedef int DiceValue; // [2, 12], representing result of a double dice roll
typedef int ArcType;
typedef int CampusType;
typedef int bool;
typedef unsigned long size_t;

typedef struct _Game Game;

#endif
