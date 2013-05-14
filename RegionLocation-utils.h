#ifndef REGIONLOCATION_UTILS_H
#define REGIONLOCATION_UTILS_H

#include "Game-redefines.h"
#include "Map.h"

bool isExistentRegion(RegionLocation location);
bool isLandRegion(RegionLocation location);
bool isRegionsEqual(RegionLocation a, RegionLocation b);
bool isRegionsAdjacent(RegionLocation a, RegionLocation b);

// Takes UP, UP_RIGHT, DOWN_RIGHT, DOWN, DOWN_LEFT, UP_LEFT only
RegionLocation getAdjacentRegion(RegionLocation location, Direction direction);

#endif
