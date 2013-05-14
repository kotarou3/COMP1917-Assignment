#ifndef VERTEXLOCATION_UTILS_H
#define VERTEXLOCATION_UTILS_H

#include "Game-redefines.h"
#include "Map.h"

#define VERTEX_LEFT 0 // Left of a region
#define VERTEX_RIGHT 1 // Right of a region
typedef unsigned char VertexType;

bool isValidVertex(VertexLocation location);
bool isVerticesEqual(VertexLocation a, VertexLocation b);
VertexType getVertexType(VertexLocation location);

// Takes UP_RIGHT, RIGHT, DOWN_RIGHT, DOWN_LEFT, LEFT, UP_LEFT only
VertexLocation getAdjacentVertexFromRegion(RegionLocation location, Direction direction);

// Treats RIGHT, UP_RIGHT and DOWN_RIGHT the same, LEFT, UP_LEFT and DOWN_LEFT the same
// and doesn't accept UP or DOWN since arcs are never up/down oriented
VertexLocation getAdjacentVertexFromEdge(EdgeLocation location, Direction direction);

typedef struct _SurroundingVerticesFromVertex {
    VertexLocation locations[3];
} SurroundingVerticesFromVertex;
SurroundingVerticesFromVertex getSurroundingVerticesFromVertex(VertexLocation location);

#endif
