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
#ifdef INCLUDE_MECHANICALTURK_H
    #include "mechanicalTurk.h"
#endif

#undef region
#undef arc
#undef vertex
#undef degree
#undef uni
#undef action
#undef _game
#undef Game

#define NUM_PLAYERS NUM_UNIS
#define NUM_ALL_REGIONS (NUM_REGIONS + 18)
#define NUM_LAND_REGIONS NUM_REGIONS
#define NUM_EDGES 72
#define NUM_VERTICES 54

#define LAND_RADIUS 2 // Max distance of a land hexagon from the origin
#define REGION_RADIUS (LAND_RADIUS + 1) // Max distance of any hexagon from the origin

#define DEGREE_THD STUDENT_THD
#define DEGREE_BPS STUDENT_BPS
#define DEGREE_BQN STUDENT_BQN
#define DEGREE_MJ STUDENT_MJ
#define DEGREE_MTV STUDENT_MTV
#define DEGREE_MMONEY STUDENT_MMONEY

// Starting number of each resource
#define START_NUM_THD 0
#define START_NUM_BPS 3
#define START_NUM_BQN 3
#define START_NUM_MJ 1
#define START_NUM_MTV 1
#define START_NUM_MMONEY 1

#define START_NUM_PUBLICATIONS 0
#define START_NUM_PATENTS 0

#define EXCHANGE_RATE_NORMAL 3
#define EXCHANGE_RATE_LOW 2

#define false 0
#define true !0

typedef int DiceValue; // [2, 12], representing result of a double dice roll
typedef int ArcType;
typedef int CampusType;
typedef int bool;
typedef unsigned long size_t;

typedef struct _Game Game;

#endif
