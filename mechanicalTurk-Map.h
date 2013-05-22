#ifndef MECHANICALTURK_MAP_H
#define MECHANICALTURK_MAP_H

#include "Game-redefines.h"
#include "EdgeLocation-utils.h"
#include "VertexLocation-utils.h"

typedef struct _MechanicalTurkRegion MechanicalTurkRegion;
typedef struct _MechanicalTurkEdge MechanicalTurkEdge;
typedef struct _MechanicalTurkVertex MechanicalTurkVertex;

struct _MechanicalTurkRegion {
    RegionLocation location;
    bool isSea;
    DegreeType generatedDegree;
    DiceValue diceValue; // Number on the hexagon
};

struct _MechanicalTurkEdge {
    EdgeLocation location;

    MechanicalTurkRegion* adjacentRegions[2];
    MechanicalTurkVertex* adjacentVertices[NUM_SURROUNDING_VERTICES_FROM_EDGE];
    size_t adjacentVertexCount;

    bool isOwned;
    PlayerId owner;
};

struct _MechanicalTurkVertex {
    VertexLocation location;

    MechanicalTurkRegion* adjacentRegions[3];
    MechanicalTurkEdge* adjacentEdges[NUM_SURROUNDING_EDGES_FROM_VERTEX];
    size_t adjacentEdgeCount;

    bool isOwned;
    bool isGo8Campus;
    PlayerId owner;

    double desirability;
};

typedef struct _MechanicalTurkMap {
    MechanicalTurkRegion regions[NUM_ALL_REGIONS];
    MechanicalTurkEdge edges[NUM_EDGES];
    MechanicalTurkVertex vertices[NUM_VERTICES];
} MechanicalTurkMap;

void constructMechanicalTurkMap(MechanicalTurkMap* map, Game* game);
void destroyMechanicalTurkMap(MechanicalTurkMap* map);

MechanicalTurkRegion* getMechanicalTurkRegion(MechanicalTurkMap* map, RegionLocation location, bool isFatalOnNotFound);
MechanicalTurkEdge* getMechanicalTurkEdge(MechanicalTurkMap* map, EdgeLocation location, bool isFatalOnNotFound);
MechanicalTurkVertex* getMechanicalTurkVertex(MechanicalTurkMap* map, VertexLocation location, bool isFatalOnNotFound);

#endif
