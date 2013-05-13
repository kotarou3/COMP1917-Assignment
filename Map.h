#ifndef MAP_H
#define MAP_H

#include "Game-redefines.h"

#define NUM_ALL_REGIONS (NUM_REGIONS + 18)
#define NUM_LAND_REGIONS NUM_REGIONS
#define NUM_EDGES 72
#define NUM_VERTICES 54

#define DEGREE_THD STUDENT_THD
#define DEGREE_BPS STUDENT_BPS
#define DEGREE_BQN STUDENT_BQN
#define DEGREE_MJ STUDENT_MJ
#define DEGREE_MTV STUDENT_MTV
#define DEGREE_MMONEY STUDENT_MMONEY

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

#define NUM_DIRECTIONS_REGION_FROM_REGION 6
#define NUM_DIRECTIONS_REGION_FROM_EDGE 2
#define NUM_DIRECTIONS_REGION_FROM_VERTEX 3
#define NUM_DIRECTIONS_EDGE_FROM_REGION 6
#define NUM_DIRECTIONS_EDGE_FROM_EDGE 4
#define NUM_DIRECTIONS_EDGE_FROM_VERTEX 3
#define NUM_DIRECTIONS_VERTEX_FROM_REGION 6
#define NUM_DIRECTIONS_VERTEX_FROM_EDGE 2
#define NUM_DIRECTIONS_VERTEX_FROM_VERTEX 3

typedef struct {
    Direction regionFromRegion[NUM_DIRECTIONS_REGION_FROM_REGION];
    Direction regionFromEdge[NUM_DIRECTIONS_REGION_FROM_EDGE];
    Direction regionFromVertex[NUM_DIRECTIONS_REGION_FROM_VERTEX];
    Direction edgeFromRegion[NUM_DIRECTIONS_EDGE_FROM_REGION];
    Direction edgeFromEdge[NUM_DIRECTIONS_EDGE_FROM_EDGE];
    Direction edgeFromVertex[NUM_DIRECTIONS_EDGE_FROM_VERTEX];
    Direction vertexFromRegion[NUM_DIRECTIONS_VERTEX_FROM_REGION];
    Direction vertexFromEdge[NUM_DIRECTIONS_VERTEX_FROM_EDGE];
    Direction vertexFromVertex[NUM_DIRECTIONS_VERTEX_FROM_VERTEX];
} ValidDirections;

extern const ValidDirections validDirections;

typedef struct {
    RegionLocation location;
    bool isSea;
    DegreeType generatedDegree;
    DiceValue diceValue; // Number on the hexagon
} Region;

typedef struct {
    EdgeLocation location;
    bool isOwned;
    PlayerId owner;
} Edge;

typedef struct {
    VertexLocation location;
    bool isOwned;
    bool isGo8Campus;
    PlayerId owner;
} Vertex;

typedef struct {
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

// Takes UP, UP_RIGHT, DOWN_RIGHT, DOWN, DOWN_LEFT, UP_LEFT only
RegionLocation getAdjacentRegion(RegionLocation location, Direction direction);
// Treats UP, UP_RIGHT and UP_LEFT the same, DOWN, DOWN_LEFT and DOWN_RIGHT the same
// and doesn't accept LEFT or RIGHT since arcs are never up/down oriented
RegionLocation getAdjacentRegionFromEdge(EdgeLocation location, Direction direction);
// Treats UP_RIGHT and RIGHT the same, DOWN_RIGHT and DOWN_LEFT the same, LEFT and UP_LEFT the same
// because only two types of vertices, and going clockwise these positions are equivalent
RegionLocation getAdjacentRegionFromVertex(VertexLocation location, Direction direction);

// Takes UP, UP_RIGHT, DOWN_RIGHT, DOWN, DOWN_LEFT, LEFT only
EdgeLocation getAdjacentEdgeFromRegion(RegionLocation location, Direction direction);
// Treats UP_RIGHT and RIGHT the same, DOWN_RIGHT and DOWN_LEFT the same, LEFT and UP_LEFT the same
// because two types of vertices and going clockwise these are equivalent
EdgeLocation getAdjacentEdgeFromVertex(VertexLocation location, Direction direction);

// Takes UP_RIGHT, RIGHT, DOWN_RIGHT, DOWN_LEFT, LEFT, UP_LEFT only
VertexLocation getAdjacentVertexFromRegion(RegionLocation location, Direction direction);
// Treats RIGHT, UP_RIGHT and DOWN_RIGHT the same, LEFT, UP_LEFT and DOWN_LEFT the same
// and doesn't accept UP or DOWN since arcs are never up/down oriented
VertexLocation getAdjacentVertexFromEdge(EdgeLocation location, Direction direction);

bool isRegionsEqual(RegionLocation a, RegionLocation b);
bool isEdgesEqual(EdgeLocation a, EdgeLocation b);
bool isVerticesEqual(VertexLocation a, VertexLocation b);

#endif
