#ifndef MAP_H
#define MAP_H

#include "Game-redefines.h"

#define NUM_ALL_REGIONS (NUM_REGIONS + 18)
#define NUM_LAND_REGIONS NUM_REGIONS
#define NUM_EDGES 72
#define NUM_VERTICES 54

#define TRAINING_CENTRE_MTV_1 ((VertexLocation){{-2, 3}, {-1, 3}, {-1, 2}})
#define TRAINING_CENTRE_MTV_2 ((VertexLocation){{0, 2}, {-1, 3}, {-1, 2}})
#define TRAINING_CENTRE_MMONEY_1 ((VertexLocation){{0, 2}, {1, 2}, {1, 1}})
#define TRAINING_CENTRE_MMONEY_2 ((VertexLocation){{2, 1}, {1, 2}, {1, 1}})
#define TRAINING_CENTRE_BQN_1 ((VertexLocation){{3, -1}, {1, 2}, {1, 1}})
#define TRAINING_CENTRE_BQN_2 ((VertexLocation){{2, -2}, {1, 2}, {1, 1}})
#define TRAINING_CENTRE_MJ_1 ((VertexLocation){{1, -3}, {1, -2}, {2, -3}})
#define TRAINING_CENTRE_MJ_2 ((VertexLocation){{2, -2}, {1, -2}, {2, -3}})
#define TRAINING_CENTRE_BPS_1 ((VertexLocation){{-1, -2}, {-2, -1}, {-1, -1}})
#define TRAINING_CENTRE_BPS_2 ((VertexLocation){{-2, -1}, {-2, -1}, {-1, -1}})

#define UP 1
#define DOWN 2
#define LEFT 4
#define RIGHT 8
#define UP_LEFT (UP | LEFT)
#define UP_RIGHT (UP | RIGHT)
#define DOWN_LEFT (DOWN | LEFT)
#define DOWN_RIGHT (DOWN | RIGHT)
typedef unsigned char Direction;

typedef struct _Region {
    RegionLocation location;
    bool isSea;
    DegreeType generatedDegree;
    DiceValue diceValue; // Number on the hexagon
} Region;

typedef struct _Edge {
    EdgeLocation location;
    bool isOwned;
    PlayerId owner;
} Edge;

typedef struct _Vertex {
    VertexLocation location;
    bool isOwned;
    bool isGo8Campus;
    PlayerId owner;
} Vertex;

typedef struct _Map {
    Region regions[NUM_ALL_REGIONS];
    Edge edges[NUM_EDGES];
    Vertex vertices[NUM_VERTICES];
} Map;

/* Game.h interface
bool isSea(Game* game, RegionLocation location);
DegreeType getDegree(Game* game, RegionLocation location);
DiceValue getDiceValue(Game* game, RegionLocation location);

ArcType getARC(Game* game, EdgeLocation location);
CampusType getCampus(Game* game, VertexLocation location);
*/

// Helper functions
void constructMap(Map* map, DegreeType* regionDegreeTypes, DiceValue* regionDiceValues);
void destroyMap(Map* map);

Region* getRegion(Map* map, RegionLocation location, bool isFatalOnNotFound);
Edge* getEdge(Map* map, EdgeLocation location, bool isFatalOnNotFound);
Vertex* getVertex(Map* map, VertexLocation location, bool isFatalOnNotFound);

#endif
