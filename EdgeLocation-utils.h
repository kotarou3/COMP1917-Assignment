#ifndef EDGELOCATION_UTILS_H
#define EDGELOCATION_UTILS_H

#include "Game-redefines.h"

#define EDGE_FLAT 0
#define EDGE_POSITIVE 1 // Positive gradient
#define EDGE_NEGATIVE 2 // Negative gradient
typedef unsigned char EdgeType;

bool isValidEdge(EdgeLocation location);
bool isEdgesEqual(EdgeLocation a, EdgeLocation b);
EdgeType getEdgeType(EdgeLocation location);

#endif
