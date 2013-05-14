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

// Treats UP, UP_RIGHT and UP_LEFT the same, DOWN, DOWN_LEFT and DOWN_RIGHT the same
// and doesn't accept LEFT or RIGHT since arcs are never up/down oriented
RegionLocation getAdjacentRegionFromEdge(EdgeLocation location, Direction direction);

// Treats UP_RIGHT and RIGHT the same, DOWN_RIGHT and DOWN_LEFT the same, LEFT and UP_LEFT the same
// because only two types of vertices, and going clockwise these positions are equivalent
RegionLocation getAdjacentRegionFromVertex(VertexLocation location, Direction direction);

#endif
