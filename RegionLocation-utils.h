#ifndef REGIONLOCATION_UTILS_H
#define REGIONLOCATION_UTILS_H

#include "Game-redefines.h"

#define UP 1
#define DOWN 2
#define LEFT 4
#define RIGHT 8
#define UP_LEFT (UP | LEFT)
#define UP_RIGHT (UP | RIGHT)
#define DOWN_LEFT (DOWN | LEFT)
#define DOWN_RIGHT (DOWN | RIGHT)
typedef unsigned char Direction;

bool isExistentRegion(RegionLocation location);
bool isLandRegion(RegionLocation location);
bool isRegionsEqual(RegionLocation a, RegionLocation b);
bool isRegionsAdjacent(RegionLocation a, RegionLocation b);

// Takes UP, UP_RIGHT, DOWN_RIGHT, DOWN, DOWN_LEFT, UP_LEFT only
RegionLocation getAdjacentRegion(RegionLocation location, Direction direction);

#endif
