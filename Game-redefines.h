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

// Rename function prototypes in favour for better typed ones (but interoperable)
#define newGame unused_newGame
#define disposeGame unused_disposeGame
#define getTurnNumber unused_getTurnNumber
#define getWhoseTurn unused_getWhoseTurn
#define getKPIpoints unused_getKPIpoints
#define getMostARCs unused_getMostARCs
#define getMostPublications unused_getMostPublications
#define isLegalAction unused_isLegalAction
#define makeAction unused_makeAction
#define throwDice unused_throwDice
#define isSea unused_isSea
#define getDegree unused_getDegree
#define getDiceValue unused_getDiceValue
#define getARC unused_getARC
#define getCampus unused_getCampus
#define getARCs unused_getARCs
#define getCampuses unused_getCampuses
#define getGO8s unused_getGO8s
#define getStudents unused_getStudents
#define getExchangeRate unused_getExchangeRate
#define getPublications unused_getPublications
#define getIPs unused_getIPs

#include "Game.h"

#undef region
#undef arc
#undef vertex
#undef degree
#undef uni
#undef action
#undef _game
#undef Game
#undef newGame
#undef disposeGame
#undef getTurnNumber
#undef getWhoseTurn
#undef getKPIpoints
#undef getMostARCs
#undef getMostPublications
#undef isLegalAction
#undef makeAction
#undef throwDice
#undef isSea
#undef getDegree
#undef getDiceValue
#undef getARC
#undef getCampus
#undef getARCs
#undef getCampuses
#undef getGO8s
#undef getStudents
#undef getExchangeRate
#undef getPublications
#undef getIPs

#define NUM_PLAYERS NUM_UNIS

#define false 0
#define true !0

typedef unsigned char DiceValue; // [2, 12], representing result of a double dice roll
typedef unsigned char ArcType;
typedef unsigned char CampusType;
typedef unsigned char bool;
typedef unsigned long size_t;

typedef struct _Game Game;

#endif
