#ifndef EDGELOCATION_UTILS_H
#define EDGELOCATION_UTILS_H

#include "Game-redefines.h"
#include "Map.h"

#define EDGE_FLAT 0
#define EDGE_POSITIVE 1 // Positive gradient
#define EDGE_NEGATIVE 2 // Negative gradient
typedef unsigned char EdgeType;

bool isValidEdge(EdgeLocation location);
bool isEdgesEqual(EdgeLocation a, EdgeLocation b);
EdgeType getEdgeType(EdgeLocation location);

// Takes UP, UP_RIGHT, DOWN_RIGHT, DOWN, DOWN_LEFT, LEFT only
EdgeLocation getAdjacentEdgeFromRegion(RegionLocation location, Direction direction);

// Treats UP_RIGHT and RIGHT the same, DOWN_RIGHT and DOWN_LEFT the same, LEFT and UP_LEFT the same
// because two types of vertices and going clockwise these are equivalent
EdgeLocation getAdjacentEdgeFromVertex(VertexLocation location, Direction direction);

#endif
