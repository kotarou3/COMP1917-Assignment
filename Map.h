#ifndef MAP_H
#define MAP_H

#include "Game-redefines.h"

#define NUM_EDGES 72
#define NUM_VERTICIES 54

#define UP 1
#define DOWN 2
#define LEFT 4
#define RIGHT 8
#define UP_LEFT (UP | LEFT)
#define UP_RIGHT (UP | RIGHT)
#define DOWN_LEFT (DOWN | LEFT)
#define DOWN_RIGHT (DOWN | RIGHT)
typedef unsigned char Direction;

typedef struct {
    RegionLocation location;
    bool isSea;
    DiceValue diceValue; // Number on the hexagon
    DegreeType generatedDegree;
} Region;

typedef struct {
    EdgeLocation location;
    bool isOwned;
    PlayerId owner;
    ArcType arcType;
} Edge;

typedef struct {
    VertexLocation location;
    bool isOwned;
    PlayerId owner;
    CampusType campusType;
} Vertex;

typedef struct {
    Region regions[NUM_REGIONS];
    Edge edges[NUM_EDGES];
    Vertex verticies[NUM_VERTICIES];
} Map;

// Game.h interface
bool isSea(Game* game, RegionLocation location);
DegreeType getDegree(Game* game, RegionLocation location);
DiceValue getDiceValue(Game* game, RegionLocation location);

ArcType getARC(Game* game, EdgeLocation location);
CampusType getCampus(Game* game, VertexLocation location);

// Helper functions
void constructMap(Map* map, DegreeType* regionDegreeTypes, DiceValue* regionDiceValues);
void destroyMap(Map* map);

Region* getRegion(Map* map, RegionLocation location);
Edge* getEdge(Map* map, EdgeLocation location);
Vertex* getVertex(Map* map, VertexLocation location);

// Takes UP, UP_RIGHT, DOWN_RIGHT, DOWN, DOWN_LEFT, UP_LEFT only
RegionLocation getAdjacentRegion(Map* map, RegionLocation location, Direction direction);
// Treats UP, UP_RIGHT and UP_LEFT the same, DOWN, DOWN_LEFT and DOWN_RIGHT the same
// and doesn't accept LEFT or RIGHT since arcs are never up/down oriented
RegionLocation getAdjacentRegionFromEdge(Map* map, EdgeLocation location, Direction direction);
// Treats UP_RIGHT and RIGHT the same, DOWN_RIGHT and DOWN_LEFT the same, LEFT and UP_LEFT the same
// because only two types of vertices, and going clockwise these positions are equivalent
RegionLocation getAdjacentRegionFromVertex(Map* map, VertexLocation location, Direction direction);

// Takes UP, UP_RIGHT, DOWN_RIGHT, DOWN, DOWN_LEFT, LEFT only
EdgeLocation getAdjacentEdgeFromRegion(Map* map, RegionLocation location, Direction direction);
// Treats UP_RIGHT and RIGHT the same, DOWN_RIGHT and DOWN_LEFT the same, LEFT and UP_LEFT the same
// because two types of vertices and going clockwise these are equivalent
EdgeLocation getAdjacentEdgeFromVertex(Map* map, VertexLocation location, Direction direction);

// Takes UP_RIGHT, RIGHT, DOWN_RIGHT, DOWN_LEFT, LEFT, UP_LEFT only
VertexLocation getAdjacentVertexFromRegion(Map* map, RegionLocation location, Direction direction);
// Treats RIGHT, UP_RIGHT and DOWN_RIGHT the same, LEFT, UP_LEFT and DOWN_LEFT the same
// and doesn't accept UP or DOWN since arcs are never up/down oriented
VertexLocation getAdjacentVertexFromEdge(Map* map, EdgeLocation location, Direction direction);

#endif
