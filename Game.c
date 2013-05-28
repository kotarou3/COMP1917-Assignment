// Game.c

// By UNSW and Caringbah tutorial groups, 16/05/13

// Implements the Game type for a game of Knowledge Island
// And the functions to interact with it abstractly

// Make sure the size_t definition in stdlib.h doesn't conflict with our's
#define size_t _size_t
#include <stdlib.h>
#undef size_t

// ====================================================
// File: Game-redefines.h
// ====================================================
#ifndef GAME_REDEFINES_H
#define GAME_REDEFINES_H

// Game.h wrapper to redefine the crap type names and to add some of our own

// Give better names to the structs and typedefs
#define region RegionLocation
#define arc EdgeLocation
#define vertex VertexLocation
#define degree DegreeType
#define uni PlayerId
#define action Action

// Ignore a conflicting typedef
#define _game _Game
#define Game unused_Game

#include "Game.h"

#undef region
#undef arc
#undef vertex
#undef degree
#undef uni
#undef action
#undef _game
#undef Game

#define NUM_PLAYERS NUM_UNIS
#define NUM_ALL_REGIONS (NUM_REGIONS + 18)
#define NUM_LAND_REGIONS NUM_REGIONS
#define NUM_EDGES 72
#define NUM_VERTICES 54

#define LAND_RADIUS 2 // Max distance of a land hexagon from the origin
#define REGION_RADIUS (LAND_RADIUS + 1) // Max distance of any hexagon from the origin

#define DEGREE_THD STUDENT_THD
#define DEGREE_BPS STUDENT_BPS
#define DEGREE_BQN STUDENT_BQN
#define DEGREE_MJ STUDENT_MJ
#define DEGREE_MTV STUDENT_MTV
#define DEGREE_MMONEY STUDENT_MMONEY

// Starting number of each resource
#define START_NUM_THD 0
#define START_NUM_BPS 3
#define START_NUM_BQN 3
#define START_NUM_MJ 1
#define START_NUM_MTV 1
#define START_NUM_MMONEY 1

#define START_NUM_PUBLICATIONS 0
#define START_NUM_PATENTS 0

#define EXCHANGE_RATE_NORMAL 3
#define EXCHANGE_RATE_LOW 2

#define false 0
#define true !0

typedef int DiceValue; // [2, 12], representing result of a double dice roll
typedef int ArcType;
typedef int CampusType;
typedef int bool;
typedef unsigned long size_t;

typedef struct _Game Game;

#endif

// ====================================================
// File: Map.h
// ====================================================
#ifndef MAP_H
#define MAP_H

//#include "Game-redefines.h"

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

// ====================================================
// File: University.h
// ====================================================
#ifndef UNIVERSITY_H
#define UNIVERSITY_H

//#include "Game-redefines.h"
//#include "Map.h"

#define NORMAL_CAMPUS_COST ((StudentCount){0, 1, 1, 1, 1, 0})
#define GO8_CAMPUS_COST ((StudentCount){0, 0, 0, 2, 0, 3})
#define ARC_COST ((StudentCount){0, 1, 1, 0, 0, 0})
#define SPINOFF_COST ((StudentCount){0, 0, 0, 1, 1, 1})

typedef struct _StudentCount {
    size_t thd;
    size_t bps;
    size_t bqn;
    size_t mj;
    size_t mtv;
    size_t mmoney;
} StudentCount;

typedef struct _University {
    PlayerId playerId;

    StudentCount studentCount;

    size_t publicationCount;
    size_t patentCount;

    size_t ownedArcCount;
    Edge** ownedArcs;

    size_t ownedCampusCount;
    Vertex** ownedCampuses;
} University;

/* Game.h functions
int getARCs(Game* game, PlayerId player);
int getCampuses(Game* game, PlayerId player);
int getGO8s(Game* game, PlayerId player);

int getStudents(Game* game, PlayerId player, DegreeType discipline);
int getExchangeRate(Game* game, PlayerId player, DegreeType from, DegreeType to);

int getPublications(Game* game, PlayerId player);
int getIPs(Game* game, PlayerId player);
*/

// Helper functions
void constructUniversity(University* university, PlayerId player);
void destroyUniversity(University* university);

size_t getNormalCampusCount(const University* university);
size_t getGo8CampusCount(const University* university);
size_t getStudentCount(const University* university, DegreeType type);
size_t getStudentExchangeRate(const University* university, DegreeType from, DegreeType to);

bool isPossibleAction(University* university, Map* map, Action action);
void doAction(University* university, Map* map, Action action);

void buyArc(University* university, Edge* location);
void buyCampus(University* university, Vertex* location, bool isGo8, bool isStarting);
void retrainStudents(University* university, DegreeType from, DegreeType to);

#endif

// ====================================================
// File: Game-wrapper.h
// ====================================================
#ifndef GAME_WRAPPER_H
#define GAME_WRAPPER_H

//#include "Game-redefines.h"
//#include "Map.h"
//#include "University.h"

#define UNI_A_START_CAMPUS_0 ((VertexLocation){{-1, 3}, {0, 2}, {0, 3}})
#define UNI_A_START_CAMPUS_1 ((VertexLocation){{0, -3}, {0, -2}, {1, -3}})
#define UNI_B_START_CAMPUS_0 ((VertexLocation){{-3, 2}, {-3, 3}, {-2, 2}})
#define UNI_B_START_CAMPUS_1 ((VertexLocation){{2, -2}, {3, -3}, {3, -2}})
#define UNI_C_START_CAMPUS_0 ((VertexLocation){{-3, 0}, {-2, -1}, {-2, 0}})
#define UNI_C_START_CAMPUS_1 ((VertexLocation){{2, 0}, {2, 1}, {3, 0}})

#define KPI_PER_ARC 2
#define KPI_PER_CAMPUS 10
#define KPI_PER_GO8 20
#define KPI_PER_PATENT 10
#define KPI_FOR_MOST_PUBS 10
#define KPI_FOR_MOST_ARCS 10

#define MAX_GO8_CAMPUSES 8

#define STUPID_DICE_VALUE_RULE 7

struct _Game {
    int currentTurn;
    Map map;
    University universities[NUM_PLAYERS];

    size_t totalGo8CampusCount;

    size_t mostPublications;
    PlayerId mostPublicationsPlayer;

    size_t mostArcs;
    PlayerId mostArcsPlayer;
};

/* Game.h interface
Game* newGame(DegreeType regionDegreeTypes[], DiceValue regionDiceValues[]);
void disposeGame(Game* game);

int getTurnNumber(Game* game);
PlayerId getWhoseTurn(Game* game);

int getKPIpoints(Game* game, PlayerId player);
PlayerId getMostPublications(Game* game);
PlayerId getMostARCs(Game* game);

bool isLegalAction(Game* game, Action action);
void makeAction(Game* game, Action action);
void throwDice(Game* game, DiceValue diceValue);
*/

// Helper functions
void constructGame(Game* game, DegreeType* regionDegreeTypes, DiceValue* regionDiceValues);
void destroyGame(Game* game);

University* getOwnedUniversity(Game* game, PlayerId player, bool isFatalOnNotFound);

#endif

// ====================================================
// File: RegionLocation-utils.h
// ====================================================
#ifndef REGIONLOCATION_UTILS_H
#define REGIONLOCATION_UTILS_H

//#include "Game-redefines.h"

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

// ====================================================
// File: EdgeLocation-utils.h
// ====================================================
#ifndef EDGELOCATION_UTILS_H
#define EDGELOCATION_UTILS_H

//#include "Game-redefines.h"

#define EDGE_FLAT 0
#define EDGE_POSITIVE 1 // Positive gradient
#define EDGE_NEGATIVE 2 // Negative gradient
typedef unsigned char EdgeType;

bool isValidEdge(EdgeLocation location);
bool isEdgesEqual(EdgeLocation a, EdgeLocation b);
EdgeType getEdgeType(EdgeLocation location);

EdgeLocation getEdgeFromTwoVertices(VertexLocation a, VertexLocation b);

#define NUM_SURROUNDING_EDGES_FROM_EDGE 4
typedef struct _SurroundingEdgesFromEdge {
    EdgeLocation locations[NUM_SURROUNDING_EDGES_FROM_EDGE];
} SurroundingEdgesFromEdge;
SurroundingEdgesFromEdge getSurroundingEdgesFromEdge(EdgeLocation location);

#define NUM_SURROUNDING_EDGES_FROM_VERTEX 3
typedef struct _SurroundingEdgesFromVertex {
    EdgeLocation locations[NUM_SURROUNDING_EDGES_FROM_VERTEX];
} SurroundingEdgesFromVertex;
SurroundingEdgesFromVertex getSurroundingEdgesFromVertex(VertexLocation location);

#endif

// ====================================================
// File: VertexLocation-utils.h
// ====================================================
#ifndef VERTEXLOCATION_UTILS_H
#define VERTEXLOCATION_UTILS_H

//#include "Game-redefines.h"

#define VERTEX_LEFT 0 // Left of a region
#define VERTEX_RIGHT 1 // Right of a region
typedef unsigned char VertexType;

bool isValidVertex(VertexLocation location);
bool isVerticesEqual(VertexLocation a, VertexLocation b);
VertexType getVertexType(VertexLocation location);

#define NUM_SURROUNDING_VERTICES_FROM_REGION 6
typedef struct _SurroundingVerticesFromRegion {
    VertexLocation locations[NUM_SURROUNDING_VERTICES_FROM_REGION];
} SurroundingVerticesFromRegion;
SurroundingVerticesFromRegion getSurroundingVerticesFromRegion(RegionLocation location);

#define NUM_SURROUNDING_VERTICES_FROM_EDGE 2
typedef struct _SurroundingVerticesFromEdge {
    VertexLocation locations[NUM_SURROUNDING_VERTICES_FROM_EDGE];
} SurroundingVerticesFromEdge;
SurroundingVerticesFromEdge getSurroundingVerticesFromEdge(EdgeLocation location);

#define NUM_SURROUNDING_VERTICES_FROM_VERTEX 3
typedef struct _SurroundingVerticesFromVertex {
    VertexLocation locations[NUM_SURROUNDING_VERTICES_FROM_VERTEX];
} SurroundingVerticesFromVertex;
SurroundingVerticesFromVertex getSurroundingVerticesFromVertex(VertexLocation location);

#endif

// ====================================================
// File: Game.c
// ====================================================
#include <stdlib.h>
#include <assert.h>

//#include "Game-wrapper.h"
//#include "VertexLocation-utils.h"

Game* newGame(DegreeType regionDegreeTypes[], DiceValue regionDiceValues[]) {
    Game* game = malloc(sizeof(Game));
    assert(game != NULL);
    constructGame(game, regionDegreeTypes, regionDiceValues);
    return game;
}

void disposeGame(Game* game) {
    destroyGame(game);
    free(game);
}

int getTurnNumber(Game* game) {
    return game->currentTurn;
}

PlayerId getWhoseTurn(Game* game) {
    return (game->currentTurn + 3) % 3;
}

int getKPIpoints(Game* game, PlayerId player) {
    University* uni = getOwnedUniversity(game, player, true);
    int kpi = 0;

    kpi += uni->ownedArcCount * KPI_PER_ARC;
    kpi += getNormalCampusCount(uni) * KPI_PER_CAMPUS;
    kpi += getGo8CampusCount(uni) * KPI_PER_GO8;
    kpi += uni->patentCount * KPI_PER_PATENT;

    if (getMostARCs(game) == player) {
        kpi += KPI_FOR_MOST_ARCS;
    }
    if (getMostPublications(game) == player) {
        kpi += KPI_FOR_MOST_PUBS;
    }

    return kpi;
}

PlayerId getMostPublications(Game* game) {
    return game->mostPublicationsPlayer;
}

PlayerId getMostARCs(Game* game) {
    return game->mostArcsPlayer;
}

bool isLegalAction(Game* game, Action action) {
    // Do a bit of global validation before passing to player validation
    if (game->currentTurn < 0) {
        return false;
    }

    if (action.actionCode == BUILD_GO8 && game->totalGo8CampusCount >= MAX_GO8_CAMPUSES) {
        return false;
    }

    return isPossibleAction(getOwnedUniversity(game, getWhoseTurn(game), true),
        &game->map, action);
}

void makeAction(Game* game, Action action) {
    University* university = getOwnedUniversity(game, getWhoseTurn(game), true);
    doAction(university, &game->map, action);

    if (action.actionCode == BUILD_GO8) {
        game->totalGo8CampusCount++;
    }

    // Check for updated ARC or publication counts
    if (university->publicationCount > game->mostPublications) {
        game->mostPublications = university->publicationCount;
        game->mostPublicationsPlayer = university->playerId;
    }
    if (university->ownedArcCount > game->mostArcs) {
        game->mostArcs = university->ownedArcCount;
        game->mostArcsPlayer = university->playerId;
    }
}

void throwDice(Game* game, DiceValue diceValue) {
    game->currentTurn++;

    size_t r = 0;
    while (r < NUM_ALL_REGIONS) {
        if (game->map.regions[r].diceValue == diceValue) {
            DegreeType degreeType = game->map.regions[r].generatedDegree;
            SurroundingVerticesFromRegion vertices = getSurroundingVerticesFromRegion(game->map.regions[r].location);

            size_t v = 0;
            while (v < NUM_SURROUNDING_VERTICES_FROM_REGION) {
                Vertex* vertex = getVertex(&game->map, vertices.locations[v], true);
                if (vertex->isOwned) {
                    size_t increaseAmount = 1;
                    if (vertex->isGo8Campus) {
                        increaseAmount = 2;
                    }

                    University* university = getOwnedUniversity(game, vertex->owner, true);
                    if (degreeType == DEGREE_THD) {
                        university->studentCount.thd += increaseAmount;
                    } else if (degreeType == DEGREE_BPS) {
                        university->studentCount.bps += increaseAmount;
                    } else if (degreeType == DEGREE_BQN) {
                        university->studentCount.bqn += increaseAmount;
                    } else if (degreeType == DEGREE_MJ) {
                        university->studentCount.mj += increaseAmount;
                    } else if (degreeType == DEGREE_MTV) {
                        university->studentCount.mtv += increaseAmount;
                    } else if (degreeType == DEGREE_MMONEY) {
                        university->studentCount.mmoney += increaseAmount;
                    }
                }
                v++;
            }
        }
        r++;
    }

    if (diceValue == STUPID_DICE_VALUE_RULE) {
        size_t u = 0;
        while (u < NUM_PLAYERS) {
            University* university = &game->universities[u];

            university->studentCount.thd += university->studentCount.mmoney;
            university->studentCount.thd += university->studentCount.mtv;
            university->studentCount.mmoney = 0;
            university->studentCount.mtv = 0;

            u++;
        }
    }
}

void constructGame(Game* game, DegreeType* regionDegreeTypes, DiceValue* regionDiceValues) {
    game->currentTurn = -1;
    constructMap(&game->map, regionDegreeTypes, regionDiceValues);

    constructUniversity(&game->universities[0], UNI_A);
    constructUniversity(&game->universities[1], UNI_B);
    constructUniversity(&game->universities[2], UNI_C);

    buyCampus(getOwnedUniversity(game, UNI_A, true),
        getVertex(&game->map, UNI_A_START_CAMPUS_0, true), false, true);
    buyCampus(getOwnedUniversity(game, UNI_A, true),
        getVertex(&game->map, UNI_A_START_CAMPUS_1, true), false, true);
    buyCampus(getOwnedUniversity(game, UNI_B, true),
        getVertex(&game->map, UNI_B_START_CAMPUS_0, true), false, true);
    buyCampus(getOwnedUniversity(game, UNI_B, true),
        getVertex(&game->map, UNI_B_START_CAMPUS_1, true), false, true);
    buyCampus(getOwnedUniversity(game, UNI_C, true),
        getVertex(&game->map, UNI_C_START_CAMPUS_0, true), false, true);
    buyCampus(getOwnedUniversity(game, UNI_C, true),
        getVertex(&game->map, UNI_C_START_CAMPUS_1, true), false, true);

    game->totalGo8CampusCount = 0;
    game->mostPublications = 0;
    game->mostPublicationsPlayer = UNI_C;
    game->mostArcs = 0;
    game->mostArcsPlayer = UNI_C;
}

void destroyGame(Game* game) {
    destroyMap(&game->map);

    size_t u = 0;
    while (u < NUM_PLAYERS) {
        destroyUniversity(&game->universities[u]);
        u++;
    }
}

University* getOwnedUniversity(Game* game, PlayerId player, bool isFatalOnNotFound) {
    University* university = NULL;

    size_t u = 0;
    while (u < NUM_PLAYERS) {
        if (game->universities[u].playerId == player) {
            university = &game->universities[u];
        }
        u++;
    }
    if (isFatalOnNotFound && university == NULL) {
        assert(!"Invalid player ID");
    }
    return university;
}

// ====================================================
// File: Map.c
// ====================================================
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

//#include "Game-wrapper.h"
//#include "Map.h"
//#include "RegionLocation-utils.h"
//#include "EdgeLocation-utils.h"
//#include "VertexLocation-utils.h"

#define MAX_REGION_DISTANCE_FROM_ORIGIN 2
#define MAX_DISTANCE_FROM_ORIGIN 3

static void constructRegions(Region* regions, DegreeType* generatedDegrees, DiceValue* diceValues);
static void destroyRegions(Region* regions);
static void constructEdges(Edge* edges);
static void destroyEdges(Edge* edges);
static void constructVertices(Vertex* vertices);
static void destroyVertices(Vertex* vertices);

static void constructRegion(Region* region, RegionLocation location, DegreeType generatedDegree, DiceValue diceValue);
static void destroyRegion(Region* region);
static void constructEdge(Edge* edge, EdgeLocation location);
static void destroyEdge(Edge* edge);
static void constructVertex(Vertex* vertex, VertexLocation location);
static void destroyVertex(Vertex* vertex);

bool isSea(Game* game, RegionLocation location) {
    return getRegion(&game->map, location, true)->isSea;
}

DegreeType getDegree(Game* game, RegionLocation location) {
    return getRegion(&game->map, location, true)->generatedDegree;
}

DiceValue getDiceValue(Game* game, RegionLocation location) {
    return getRegion(&game->map, location, true)->diceValue;
}

ArcType getARC(Game* game, EdgeLocation location) {
    Edge* edge = getEdge(&game->map, location, true);
    ArcType arcType = VACANT_ARC;

    if (edge->isOwned) {
        if (edge->owner == UNI_A) {
            arcType = ARC_A;
        } else if (edge->owner == UNI_B) {
            arcType = ARC_B;
        } else if (edge->owner == UNI_C) {
            arcType = ARC_C;
        }
    }

    return arcType;
}

CampusType getCampus(Game* game, VertexLocation location) {
    Vertex* vertex = getVertex(&game->map, location, true);
    CampusType campusType = VACANT_VERTEX;

    if (vertex->isOwned) {
        if (vertex->owner == UNI_A) {
            if (vertex->isGo8Campus) {
                campusType = GO8_CAMPUS_A;
            } else {
                campusType = CAMPUS_A;
            }
        } else if (vertex->owner == UNI_B) {
            if (vertex->isGo8Campus) {
                campusType = GO8_CAMPUS_B;
            } else {
                campusType = CAMPUS_B;
            }
        } else if (vertex->owner == UNI_C) {
            if (vertex->isGo8Campus) {
                campusType = GO8_CAMPUS_C;
            } else {
                campusType = CAMPUS_C;
            }
        }
    }

    return campusType;
}

void constructMap(Map* map, DegreeType* regionDegreeTypes, DiceValue* regionDiceValues) {
    constructRegions(map->regions, regionDegreeTypes, regionDiceValues);
    constructEdges(map->edges);
    constructVertices(map->vertices);
}

void destroyMap(Map* map) {
    destroyRegions(map->regions);
    destroyEdges(map->edges);
    destroyVertices(map->vertices);
}


Region* getRegion(Map* map, RegionLocation location, bool isFatalOnNotFound) {
    size_t r = 0;
    while (r < NUM_ALL_REGIONS) {
        if (isRegionsEqual(map->regions[r].location, location)) {
            return &map->regions[r];
        }
        r++;
    }
    if (isFatalOnNotFound) {
        fprintf(stderr, "Requested invalid region {%d, %d}\n", location.x, location.y);
        assert(!"Invalid region location");
    }
    return NULL;
}

Edge* getEdge(Map* map, EdgeLocation location, bool isFatalOnNotFound) {
    size_t e = 0;
    while (e < NUM_EDGES) {
        if (isEdgesEqual(map->edges[e].location, location)) {
            return &map->edges[e];
        }
        e++;
    }
    if (isFatalOnNotFound) {
        fprintf(stderr, "Requested invalid edge {{%d, %d}, {%d, %d}}",
            location.region0.x, location.region0.y, location.region1.x, location.region1.y);
        assert(!"Invalid edge location");
    }
    return NULL;
}

Vertex* getVertex(Map* map, VertexLocation location, bool isFatalOnNotFound) {
    size_t v = 0;
    while (v < NUM_VERTICES) {
        if (isVerticesEqual(map->vertices[v].location, location)) {
            return &map->vertices[v];
        }
        v++;
    }
    if (isFatalOnNotFound) {
        fprintf(stderr, "Requested invalid vertex {{%d, %d}, {%d, %d}, {%d, %d}}",
            location.region0.x, location.region0.y,
            location.region1.x, location.region1.y,
            location.region2.x, location.region2.y);
        assert(!"Invalid vertex location");
    }
    return NULL;
}

static void constructRegions(Region* regions, DegreeType* generatedDegrees, DiceValue* diceValues) {
    // Set up regions by looping through array bottom to top, left to right
    // populating each region with the correct dice value and student type.
    // This is the order that generatedDegrees and diceValues happen to be in.
    size_t r = 0;
    size_t landRegionIndex = 0;
    RegionLocation location;
    location.x = -REGION_RADIUS;
    while (location.x <= REGION_RADIUS) {
        location.y = -REGION_RADIUS;
        while (location.y <= REGION_RADIUS) {
            // Make sure the region exists
            if (isExistentRegion(location)) {
                assert(r < NUM_ALL_REGIONS);

                if (isLandRegion(location)) {
                    assert(landRegionIndex < NUM_LAND_REGIONS);
                    constructRegion(&regions[r], location,
                        generatedDegrees[landRegionIndex],
                        diceValues[landRegionIndex]);
                    landRegionIndex++;
                } else {
                    constructRegion(&regions[r], location, 0, 0);
                }
                r++;
            }
            location.y++;
        }
        location.x++;
    }
    assert(r == NUM_ALL_REGIONS);
    assert(landRegionIndex == NUM_LAND_REGIONS);
}

static void destroyRegions(Region* regions) {
    size_t r = 0;
    while (r < NUM_ALL_REGIONS) {
        destroyRegion(&regions[r]);
        r++;
    }
}

static void constructEdges(Edge* edges) {
    // By looking at diagram on open learning, each region can define three edges:
    //  - One with the region below
    //  - One with the region below-right
    //  - One with the region up-right
    // except regions in the last column and row

    // Loop through regions (bottom + 1) to top, then left to (right - 1)
    // and add the edges for each region
    size_t e = 0;
    RegionLocation anchorRegion;
    anchorRegion.y = -REGION_RADIUS + 1;
    while (anchorRegion.y <= REGION_RADIUS) {
        anchorRegion.x = -REGION_RADIUS;
        while (anchorRegion.x <= REGION_RADIUS - 1) {
            EdgeLocation location;
            location.region0 = anchorRegion;
            location.region1 = getAdjacentRegion(anchorRegion, DOWN);

            // Make sure edge is adjacent to land
            if (isLandRegion(location.region0) || isLandRegion(location.region1)) {
                assert(e < NUM_EDGES);
                constructEdge(&edges[e], location);
                e++;
            }

            // Repeat two times for the other edges
            location.region1 = getAdjacentRegion(anchorRegion, DOWN_RIGHT);
            if (isLandRegion(location.region0) || isLandRegion(location.region1)) {
                assert(e < NUM_EDGES);
                constructEdge(&edges[e], location);
                e++;
            }

            location.region1 = getAdjacentRegion(anchorRegion, UP_RIGHT);
            if (isLandRegion(location.region0) || isLandRegion(location.region1)) {
                assert(e < NUM_EDGES);
                constructEdge(&edges[e], location);
                e++;
            }

            anchorRegion.x++;
        }
        anchorRegion.y++;
    }
    assert(e == NUM_EDGES);
}

static void destroyEdges(Edge* edges) {
    size_t e = 0;
    while (e < NUM_EDGES) {
        destroyEdge(&edges[e]);
        e++;
    }
}

static void constructVertices(Vertex* vertices) {
    // By looking at diagram on openlearning, each region can define two vertices:
    //  - One with the region below and below-right
    //  - One with the region above-right and below-right
    // except regions in the last column and row

    // Loop through regions (bottom + 1) to top, then left to (right - 1)
    // and add the vertices for each region
    size_t v = 0;
    RegionLocation anchorRegion;
    anchorRegion.y = -REGION_RADIUS + 1;
    while (anchorRegion.y <= REGION_RADIUS) {
        anchorRegion.x = -REGION_RADIUS;
        while (anchorRegion.x <= REGION_RADIUS - 1) {
            VertexLocation location;
            location.region0 = anchorRegion;
            location.region1 = getAdjacentRegion(anchorRegion, DOWN);
            location.region2 = getAdjacentRegion(anchorRegion, DOWN_RIGHT);

            // Make sure vertex is adjacent to land
            if (isLandRegion(location.region0) || isLandRegion(location.region1) || isLandRegion(location.region2)) {
                assert(v < NUM_VERTICES);
                constructVertex(&vertices[v], location);
                v++;
            }

            // Do it again for the other vertex
            location.region1 = getAdjacentRegion(anchorRegion, UP_RIGHT);
            if (isLandRegion(location.region0) || isLandRegion(location.region1) || isLandRegion(location.region2)) {
                assert(v < NUM_VERTICES);
                constructVertex(&vertices[v], location);
                v++;
            }

            anchorRegion.x++;
        }
        anchorRegion.y++;
    }
    assert(v == NUM_VERTICES);
}

static void destroyVertices(Vertex* vertices) {
    size_t v = 0;
    while (v < NUM_VERTICES) {
        destroyVertex(&vertices[v]);
        v++;
    }
}

static void constructRegion(Region* region, RegionLocation location, DegreeType generatedDegree, DiceValue diceValue) {
    region->location = location;
    region->isSea = !isLandRegion(location);

    if (!region->isSea) {
        region->generatedDegree = generatedDegree;
        region->diceValue = diceValue;
    }
}

static void destroyRegion(Region* region) {
    (void)region; // Do nothing
}

static void constructEdge(Edge* edge, EdgeLocation location) {
    edge->location = location;
    edge->isOwned = false;
}

static void destroyEdge(Edge* edge) {
    (void)edge; // Do nothing
}

static void constructVertex(Vertex* vertex, VertexLocation location) {
    vertex->location = location;
    vertex->isOwned = false;
}

static void destroyVertex(Vertex* vertex) {
    (void)vertex; // Do nothing
}

// ====================================================
// File: University.c
// ====================================================
#include <stdlib.h>
#include <assert.h>

//#include "Game-wrapper.h"
//#include "University.h"
//#include "EdgeLocation-utils.h"
//#include "VertexLocation-utils.h"

static bool isEnoughStudents(StudentCount* target, StudentCount cost);
static void modifyStudentCount(StudentCount* target, StudentCount cost);

int getARCs(Game* game, PlayerId player) {
    return (int)getOwnedUniversity(game, player, true)->ownedArcCount;
}

int getCampuses(Game* game, PlayerId player) {
    return (int)getNormalCampusCount(getOwnedUniversity(game, player, true));
}

int getGO8s(Game* game, PlayerId player) {
    return (int)getGo8CampusCount(getOwnedUniversity(game, player, true));
}

int getStudents(Game* game, PlayerId player, DegreeType discipline) {
    return (int)getStudentCount(getOwnedUniversity(game, player, true), discipline);
}

int getExchangeRate(Game* game, PlayerId player, DegreeType from, DegreeType to) {
    return (int)getStudentExchangeRate(getOwnedUniversity(game, player, true), from, to);
}

int getPublications(Game* game, PlayerId player) {
    return (int)getOwnedUniversity(game, player, true)->publicationCount;
}

int getIPs(Game* game, PlayerId player) {
    return (int)getOwnedUniversity(game, player, true)->patentCount;
}

void constructUniversity(University* university, PlayerId player) {
    university->playerId = player;

    university->studentCount.thd = START_NUM_THD;
    university->studentCount.bps = START_NUM_BPS;
    university->studentCount.bqn = START_NUM_BQN;
    university->studentCount.mj = START_NUM_MJ;
    university->studentCount.mtv = START_NUM_MTV;
    university->studentCount.mmoney = START_NUM_MMONEY;

    university->publicationCount = START_NUM_PUBLICATIONS;
    university->patentCount = START_NUM_PATENTS;

    university->ownedCampusCount = 0;
    university->ownedCampuses = malloc(0);
    university->ownedArcCount = 0;
    university->ownedArcs = malloc(0);
}

void destroyUniversity(University* university) {
    free(university->ownedCampuses);
    free(university->ownedArcs);
}

size_t getNormalCampusCount(const University* university) {
    size_t count = 0;
    size_t c = 0;
    while (c < university->ownedCampusCount) {
        if (!university->ownedCampuses[c]->isGo8Campus) {
            count++;
        }
        c++;
    }
    return count;
}

size_t getGo8CampusCount(const University* university) {
    size_t count = 0;
    size_t c = 0;
    while (c < university->ownedCampusCount) {
        if (university->ownedCampuses[c]->isGo8Campus) {
            count++;
        }
        c++;
    }
    return count;
}

size_t getStudentCount(const University* university, DegreeType type) {
    size_t students = 0;

    if (type == DEGREE_THD) {
        students = university->studentCount.thd;
    } else if (type == DEGREE_BPS) {
        students = university->studentCount.bps;
    } else if (type == DEGREE_BQN) {
        students = university->studentCount.bqn;
    } else if (type == DEGREE_MJ) {
        students = university->studentCount.mj;
    } else if (type == DEGREE_MTV) {
        students = university->studentCount.mtv;
    } else if (type == DEGREE_MMONEY) {
        students = university->studentCount.mmoney;
    }

    return students;
}

size_t getStudentExchangeRate(const University* university, DegreeType from, DegreeType to) {
    // Check all the owned campuses for the matching training center
    VertexLocation centre1, centre2;
    if (from == DEGREE_BPS) {
        centre1 = TRAINING_CENTRE_BPS_1;
        centre2 = TRAINING_CENTRE_BPS_2;
    } else if (from == DEGREE_BQN) {
        centre1 = TRAINING_CENTRE_BQN_1;
        centre2 = TRAINING_CENTRE_BQN_2;
    } else if (from == DEGREE_MJ) {
        centre1 = TRAINING_CENTRE_MJ_1;
        centre2 = TRAINING_CENTRE_MJ_2;
    } else if (from == DEGREE_MTV) {
        centre1 = TRAINING_CENTRE_MTV_1;
        centre2 = TRAINING_CENTRE_MTV_2;
    } else if (from == DEGREE_MMONEY) {
        centre1 = TRAINING_CENTRE_MMONEY_1;
        centre2 = TRAINING_CENTRE_MMONEY_2;
    } else {
        return EXCHANGE_RATE_NORMAL;
    }

    size_t c = 0;
    while (c < university->ownedCampusCount) {
        if (isVerticesEqual(university->ownedCampuses[c]->location, centre1) ||
            isVerticesEqual(university->ownedCampuses[c]->location, centre2)) {
            return EXCHANGE_RATE_LOW;
        }
        c++;
    }

    (void)to; // Shut up compiler warning
    return EXCHANGE_RATE_NORMAL;
}

bool isPossibleAction(University* university, Map* map, Action action) {
    if (action.actionCode == PASS) {
        return true;
    } else if (action.actionCode == BUILD_CAMPUS) {
        // Check for enough resources
        if (!isEnoughStudents(&university->studentCount, NORMAL_CAMPUS_COST)) {
            return false;
        }

        // Make sure the target vertex exists and is not already owned
        Vertex* targetVertex = getVertex(map, action.targetVertex, false);
        if (targetVertex == NULL || targetVertex->isOwned) {
            return false;
        }

        // Check for no adjacent campus
        SurroundingVerticesFromVertex vertices = getSurroundingVerticesFromVertex(targetVertex->location);
        size_t v = 0;
        while (v < NUM_SURROUNDING_VERTICES_FROM_VERTEX) {
            Vertex* testVertex = getVertex(map, vertices.locations[v], false);
            if (testVertex != NULL && testVertex->isOwned) {
                return false;
            }
            v++;
        }

        // Check for adjacent ARC owned by the player
        SurroundingEdgesFromVertex edges = getSurroundingEdgesFromVertex(targetVertex->location);
        bool isMatchingArc = false;
        size_t e = 0;
        while (e < NUM_SURROUNDING_EDGES_FROM_VERTEX && !isMatchingArc) {
            Edge* testEdge = getEdge(map, edges.locations[e], false);
            if (testEdge != NULL && testEdge->isOwned && testEdge->owner == university->playerId) {
                isMatchingArc = true;
            }
            e++;
        }
        if (!isMatchingArc) {
            return false;
        }

        return true;
    } else if (action.actionCode == BUILD_GO8) {
        // Check for enough resources
        if (!isEnoughStudents(&university->studentCount, GO8_CAMPUS_COST)) {
            return false;
        }

        // Make sure the target vertex exists and is already owned by the player
        Vertex* targetVertex = getVertex(map, action.targetVertex, false);
        if (targetVertex == NULL || !targetVertex->isOwned || targetVertex->owner != university->playerId) {
            return false;
        }

        return true;
    } else if (action.actionCode == CREATE_ARC) {
        // Check for enough resources
        if (!isEnoughStudents(&university->studentCount, ARC_COST)) {
            return false;
        }

        // Make sure the target edge exists and is not already owned
        Edge* targetEdge = getEdge(map, action.targetARC, false);
        if (targetEdge == NULL || targetEdge->isOwned) {
            return false;
        }

        // Check for adjacent ARC or campus owned by the player
        SurroundingVerticesFromEdge vertices = getSurroundingVerticesFromEdge(targetEdge->location);
        SurroundingEdgesFromEdge edges = getSurroundingEdgesFromEdge(targetEdge->location);
        bool isMatchingCampus = false;
        bool isMatchingArc = false;
        size_t v = 0;
        while (v < NUM_SURROUNDING_VERTICES_FROM_EDGE && !isMatchingCampus) {
            Vertex* testVertex = getVertex(map, vertices.locations[v], false);
            if (testVertex != NULL && testVertex->isOwned && testVertex->owner == university->playerId) {
                isMatchingCampus = true;
            }
            v++;
        }
        size_t e = 0;
        while (e < NUM_SURROUNDING_EDGES_FROM_EDGE && !isMatchingArc) {
            Edge* testEdge = getEdge(map, edges.locations[e], false);
            if (testEdge != NULL && testEdge->isOwned && testEdge->owner == university->playerId) {
                isMatchingArc = true;
            }
            e++;
        }
        if (!isMatchingCampus && !isMatchingArc) {
            return false;
        }

        return true;
    } else if (action.actionCode == START_SPINOFF) {
        // Check for enough resources
        if (!isEnoughStudents(&university->studentCount, SPINOFF_COST)) {
            return false;
        }
        return true;
    } else if (action.actionCode == RETRAIN_STUDENTS) {
        // Cannot retrain ThD students
        if (action.retrainFrom == DEGREE_THD) {
            return false;
        }

        // Check for enough resources
        if (getStudentCount(university, action.retrainFrom) <
            getStudentExchangeRate(university, action.retrainFrom, action.retrainTo)) {
            return false;
        }
        return true;
    }
    return false; // OBTAIN_PUBLICATION and OBTAIN_IP_PATENT
}

void doAction(University* university, Map* map, Action action) {
    if (action.actionCode == BUILD_CAMPUS) {
        buyCampus(university, getVertex(map, action.targetVertex, true), false, false);
    } else if (action.actionCode == BUILD_GO8) {
        buyCampus(university, getVertex(map, action.targetVertex, true), true, false);
    } else if (action.actionCode == CREATE_ARC) {
        buyArc(university, getEdge(map, action.targetARC, true));
    } else if (action.actionCode == RETRAIN_STUDENTS) {
        retrainStudents(university, action.retrainFrom, action.retrainTo);
    } else if (action.actionCode == OBTAIN_PUBLICATION || action.actionCode == OBTAIN_IP_PATENT) {
        modifyStudentCount(&university->studentCount, SPINOFF_COST);
        if (action.actionCode == OBTAIN_PUBLICATION) {
            university->publicationCount++;
        } else {
            university->patentCount++;
        }
    }
}

void buyArc(University* university, Edge* location) {
    assert(!location->isOwned);
    modifyStudentCount(&university->studentCount, ARC_COST);

    location->isOwned = true;
    location->owner = university->playerId;

    university->ownedArcCount++;
    university->ownedArcs = realloc(university->ownedArcs, sizeof(university->ownedArcs[0]) * university->ownedArcCount);
    assert(university->ownedArcs != NULL);
    university->ownedArcs[university->ownedArcCount - 1] = location;
}

void buyCampus(University* university, Vertex* location, bool isGo8, bool isStarting) {
    if (isGo8) {
        assert(location->isOwned && location->owner == university->playerId);
    } else {
        assert(!location->isOwned);
    }

    if (!isStarting) {
        if (isGo8) {
            modifyStudentCount(&university->studentCount, GO8_CAMPUS_COST);
        } else {
            modifyStudentCount(&university->studentCount, NORMAL_CAMPUS_COST);
        }
    }

    location->isOwned = true;
    location->isGo8Campus = isGo8;
    location->owner = university->playerId;

    if (!isGo8) {
        university->ownedCampusCount++;
        university->ownedCampuses = realloc(university->ownedCampuses, sizeof(university->ownedCampuses[0]) * university->ownedCampusCount);
        assert(university->ownedCampuses != NULL);
        university->ownedCampuses[university->ownedCampusCount - 1] = location;
    }
}

void retrainStudents(University* university, DegreeType from, DegreeType to) {
    size_t exchangeRate = getStudentExchangeRate(university, from, to);
    assert(getStudentCount(university, from) >= exchangeRate);

    if (from == DEGREE_BPS) {
        university->studentCount.bps -= exchangeRate;
    } else if (from == DEGREE_BQN) {
        university->studentCount.bqn -= exchangeRate;
    } else if (from == DEGREE_MJ) {
        university->studentCount.mj -= exchangeRate;
    } else if (from == DEGREE_MTV) {
        university->studentCount.mtv -= exchangeRate;
    } else if (from == DEGREE_MMONEY) {
        university->studentCount.mmoney -= exchangeRate;
    } else {
        assert(!"Invalid from degree");
    }

    if (to == DEGREE_THD) {
        university->studentCount.thd++;
    } else if (to == DEGREE_BPS) {
        university->studentCount.bps++;
    } else if (to == DEGREE_BQN) {
        university->studentCount.bqn++;
    } else if (to == DEGREE_MJ) {
        university->studentCount.mj++;
    } else if (to == DEGREE_MTV) {
        university->studentCount.mtv++;
    } else if (to == DEGREE_MMONEY) {
        university->studentCount.mmoney++;
    } else {
        assert(!"Invalid to degree");
    }
}

static bool isEnoughStudents(StudentCount* target, StudentCount cost) {
    return target->thd >= cost.thd &&
        target->bps >= cost.bps &&
        target->bqn >= cost.bqn &&
        target->mj >= cost.mj &&
        target->mtv >= cost.mtv &&
        target->mmoney >= cost.mmoney;
}

static void modifyStudentCount(StudentCount* target, StudentCount cost) {
    assert(isEnoughStudents(target, cost));

    target->thd -= cost.thd;
    target->bps -= cost.bps;
    target->bqn -= cost.bqn;
    target->mj -= cost.mj;
    target->mtv -= cost.mtv;
    target->mmoney -= cost.mmoney;
}

// ====================================================
// File: RegionLocation-utils.c
// ====================================================
#include <assert.h>

//#include "RegionLocation-utils.h"

bool isExistentRegion(RegionLocation location) {
    return -REGION_RADIUS <= location.x && location.x <= REGION_RADIUS &&
        -REGION_RADIUS <= location.y && location.y <= REGION_RADIUS &&
        -REGION_RADIUS <= location.x + location.y && location.x + location.y <= REGION_RADIUS;
}

bool isLandRegion(RegionLocation location) {
    return -LAND_RADIUS <= location.x && location.x <= LAND_RADIUS &&
        -LAND_RADIUS <= location.y && location.y <= LAND_RADIUS &&
        -LAND_RADIUS <= location.x + location.y && location.x + location.y <= LAND_RADIUS;
}

bool isRegionsEqual(RegionLocation a, RegionLocation b) {
    return a.x == b.x && a.y == b.y;
}

bool isRegionsAdjacent(RegionLocation a, RegionLocation b) {
    return (a.x == b.x && a.y + 1 == b.y) || // Up
        (a.x == b.x && a.y - 1 == b.y) || // Down
        (a.x - 1 == b.x && a.y + 1 == b.y) || // Up-left
        (a.x - 1 == b.x && a.y == b.y) || // Down-left
        (a.x + 1 == b.x && a.y == b.y) || // Up-right
        (a.x + 1 == b.x && a.y - 1 == b.y); // Down-right
}

RegionLocation getAdjacentRegion(RegionLocation location, Direction direction) {
    assert(direction & UP || direction & DOWN);

    RegionLocation result;
    result.x = location.x;
    result.y = location.y;

    if (direction & LEFT) {
        result.x--;
    } else if (direction & RIGHT) {
        result.x++;
    }

    if (direction == DOWN || direction == DOWN_RIGHT) {
        result.y--;
    } else if (direction == UP || direction == UP_LEFT) {
        result.y++;
    }

    return result;
}

// ====================================================
// File: EdgeLocation-utils.c
// ====================================================
#include <assert.h>

//#include "EdgeLocation-utils.h"
//#include "RegionLocation-utils.h"
//#include "VertexLocation-utils.h"

bool isValidEdge(EdgeLocation location) {
    return isRegionsAdjacent(location.region0, location.region1);
}

bool isEdgesEqual(EdgeLocation a, EdgeLocation b) {
    // {a.region0, a.region1} ∈ {permutations({b.region0, b.region1})}
    return (isRegionsEqual(a.region0, b.region0) && isRegionsEqual(a.region1, b.region1)) ||
        (isRegionsEqual(a.region0, b.region1) && isRegionsEqual(a.region1, b.region0));
}

EdgeType getEdgeType(EdgeLocation location) {
    assert(isValidEdge(location));
    // If one region is above another, the edge is flat.
    // Else if regions have the same y value, the edge has negative gradient.
    // Otherwise, the edge has positive gradient.

    EdgeType result = EDGE_POSITIVE;
    if (isRegionsEqual(getAdjacentRegion(location.region0, UP), location.region1) ||
        isRegionsEqual(getAdjacentRegion(location.region0, DOWN), location.region1)) {
        result = EDGE_FLAT;
    } else if (location.region0.y == location.region1.y) {
        result = EDGE_NEGATIVE;
    }
    return result;
}

EdgeLocation getEdgeFromTwoVertices(VertexLocation a, VertexLocation b) {
    RegionLocation resultRegions[2];
    size_t r = 0;

    RegionLocation aRegions[3] = {a.region0, a.region1, a.region2};
    RegionLocation bRegions[3] = {b.region0, b.region1, b.region2};
    size_t ar = 0;
    while (ar < 3) {
        size_t br = 0;
        bool isFound = false;
        while (br < 3 && !isFound) {
            if (isRegionsEqual(aRegions[ar], bRegions[br])) {
                assert(r < 2);
                resultRegions[r] = aRegions[ar];
                r++;
                isFound = true;
            }
            br++;
        }
        ar++;
    }
    assert(r == 2);

    EdgeLocation result;
    result.region0 = resultRegions[0];
    result.region1 = resultRegions[1];
    return result;
}

SurroundingEdgesFromEdge getSurroundingEdgesFromEdge(EdgeLocation location) {
    SurroundingEdgesFromEdge result;
    SurroundingVerticesFromEdge vertices = getSurroundingVerticesFromEdge(location);
    size_t e = 0;
    size_t v = 0;
    while (v < NUM_SURROUNDING_VERTICES_FROM_EDGE) {
        SurroundingEdgesFromVertex edges = getSurroundingEdgesFromVertex(vertices.locations[v]);
        size_t e2 = 0;
        while (e2 < NUM_SURROUNDING_EDGES_FROM_VERTEX) {
            if (!isEdgesEqual(edges.locations[e2], location)) {
                assert(e < NUM_SURROUNDING_EDGES_FROM_EDGE);
                result.locations[e] = edges.locations[e2];
                e++;
            }
            e2++;
        }
        v++;
    }
    assert(e == NUM_SURROUNDING_EDGES_FROM_EDGE);
    return result;
}

SurroundingEdgesFromVertex getSurroundingEdgesFromVertex(VertexLocation location) {
    assert(NUM_SURROUNDING_EDGES_FROM_VERTEX == NUM_SURROUNDING_VERTICES_FROM_VERTEX);

    SurroundingEdgesFromVertex result;
    SurroundingVerticesFromVertex vertices = getSurroundingVerticesFromVertex(location);
    size_t v = 0;
    while (v < NUM_SURROUNDING_VERTICES_FROM_VERTEX) {
        result.locations[v] = getEdgeFromTwoVertices(location, vertices.locations[v]);
        v++;
    }

    return result;
}

// ====================================================
// File: VertexLocation-utils.c
// ====================================================
#include <assert.h>

//#include "VertexLocation-utils.h"
//#include "RegionLocation-utils.h"
//#include "EdgeLocation-utils.h"

bool isValidVertex(VertexLocation location) {
    return isRegionsAdjacent(location.region0, location.region1) &&
        isRegionsAdjacent(location.region1, location.region2) &&
        isRegionsAdjacent(location.region0, location.region2);
}

bool isVerticesEqual(VertexLocation a, VertexLocation b) {
    // {a.region0, a.region1, a.region2} ∈ {permutations({b.region0, b.region1, b.region2})}
    return (isRegionsEqual(a.region0, b.region0) && isRegionsEqual(a.region1, b.region1) && isRegionsEqual(a.region2, b.region2)) ||
        (isRegionsEqual(a.region0, b.region0) && isRegionsEqual(a.region1, b.region2) && isRegionsEqual(a.region2, b.region1)) ||
        (isRegionsEqual(a.region0, b.region1) && isRegionsEqual(a.region1, b.region0) && isRegionsEqual(a.region2, b.region2)) ||
        (isRegionsEqual(a.region0, b.region1) && isRegionsEqual(a.region1, b.region2) && isRegionsEqual(a.region2, b.region0)) ||
        (isRegionsEqual(a.region0, b.region2) && isRegionsEqual(a.region1, b.region0) && isRegionsEqual(a.region2, b.region1)) ||
        (isRegionsEqual(a.region0, b.region2) && isRegionsEqual(a.region1, b.region1) && isRegionsEqual(a.region2, b.region0));
}

VertexType getVertexType(VertexLocation location) {
    assert(isValidVertex(location));
    // From examining the coordinate system, if there is only a single highest value of x,
    // it's a left vertex. Otherwise, it's a right vertex.

    int highestX = location.region0.x;
    if (location.region1.x > highestX) {
        highestX = location.region1.x;
    }
    if (location.region2.x > highestX) {
        highestX = location.region2.x;
    }

    VertexType result = VERTEX_RIGHT;
    if ((highestX == location.region0.x && highestX != location.region1.x && highestX != location.region2.x) ||
        (highestX != location.region0.x && highestX == location.region1.x && highestX != location.region2.x) ||
        (highestX != location.region0.x && highestX != location.region1.x && highestX == location.region2.x)) {
        result = VERTEX_LEFT;
    }
    return result;
}

SurroundingVerticesFromRegion getSurroundingVerticesFromRegion(RegionLocation location) {
    SurroundingVerticesFromRegion result;

    // Up-right
    result.locations[0].region0 = location;
    result.locations[0].region1 = getAdjacentRegion(location, UP);
    result.locations[0].region2 = getAdjacentRegion(location, UP_RIGHT);

    // Right
    result.locations[1].region0 = location;
    result.locations[1].region1 = getAdjacentRegion(location, UP_RIGHT);
    result.locations[1].region2 = getAdjacentRegion(location, DOWN_RIGHT);

    // Down-right
    result.locations[2].region0 = location;
    result.locations[2].region1 = getAdjacentRegion(location, DOWN);
    result.locations[2].region2 = getAdjacentRegion(location, DOWN_RIGHT);

    // Down-left
    result.locations[3].region0 = location;
    result.locations[3].region1 = getAdjacentRegion(location, DOWN);
    result.locations[3].region2 = getAdjacentRegion(location, DOWN_LEFT);

    // Left
    result.locations[4].region0 = location;
    result.locations[4].region1 = getAdjacentRegion(location, UP_LEFT);
    result.locations[4].region2 = getAdjacentRegion(location, DOWN_LEFT);

    // Up-left
    result.locations[5].region0 = location;
    result.locations[5].region1 = getAdjacentRegion(location, UP);
    result.locations[5].region2 = getAdjacentRegion(location, UP_LEFT);

    return result;
}

SurroundingVerticesFromEdge getSurroundingVerticesFromEdge(EdgeLocation location) {
    SurroundingVerticesFromEdge result;
    EdgeType type = getEdgeType(location);
    if (type == EDGE_FLAT) {
        RegionLocation upperRegion = location.region0;
        if (location.region1.y > upperRegion.y) {
            upperRegion = location.region1;
        }

        // Left
        result.locations[0].region0 = location.region0;
        result.locations[0].region1 = location.region1;
        result.locations[0].region2 = getAdjacentRegion(upperRegion, DOWN_LEFT);

        // Right
        result.locations[1].region0 = location.region0;
        result.locations[1].region1 = location.region1;
        result.locations[1].region2 = getAdjacentRegion(upperRegion, DOWN_RIGHT);
    } else if (type == EDGE_POSITIVE) {
        RegionLocation upperRegion = location.region0;
        if (location.region1.y > upperRegion.y) {
            upperRegion = location.region1;
        }

        // Up-right
        result.locations[0].region0 = location.region0;
        result.locations[0].region1 = location.region1;
        result.locations[0].region2 = getAdjacentRegion(upperRegion, UP_RIGHT);

        // Down-left
        result.locations[1].region0 = location.region0;
        result.locations[1].region1 = location.region1;
        result.locations[1].region2 = getAdjacentRegion(upperRegion, DOWN);
    } else { // EDGE_NEGATIVE
        RegionLocation upperRegion = location.region0;
        if (location.region1.x > upperRegion.x) {
            upperRegion = location.region1;
        }

        // Up-left
        result.locations[0].region0 = location.region0;
        result.locations[0].region1 = location.region1;
        result.locations[0].region2 = getAdjacentRegion(upperRegion, UP_LEFT);

        // Down-right
        result.locations[1].region0 = location.region0;
        result.locations[1].region1 = location.region1;
        result.locations[1].region2 = getAdjacentRegion(upperRegion, DOWN);
    }
    return result;
}

SurroundingVerticesFromVertex getSurroundingVerticesFromVertex(VertexLocation location) {
    // NOTE: Only two possible values for x and y (independently) for a valid vertex
    SurroundingVerticesFromVertex result;
    if (getVertexType(location) == VERTEX_LEFT) {
        RegionLocation topmostRegion = location.region0;
        if (location.region1.y > topmostRegion.y) {
            topmostRegion = location.region1;
        } else if (location.region2.y > topmostRegion.y) {
            topmostRegion = location.region2;
        }

        // Up-right
        result.locations[0].region0 = topmostRegion;
        result.locations[0].region1 = getAdjacentRegion(topmostRegion, UP_RIGHT);
        result.locations[0].region2 = getAdjacentRegion(topmostRegion, DOWN_RIGHT);

        // Left
        result.locations[1].region0 = topmostRegion;
        result.locations[1].region1 = getAdjacentRegion(topmostRegion, DOWN);
        result.locations[1].region2 = getAdjacentRegion(topmostRegion, DOWN_LEFT);

        // Down-right
        result.locations[2].region0 = result.locations[0].region2; // DOWN_RIGHT
        result.locations[2].region1 = result.locations[1].region1; // DOWN
        result.locations[2].region2 = getAdjacentRegion(result.locations[1].region1, DOWN_RIGHT);
    } else { // VERTEX_RIGHT
        RegionLocation topmostRegion = location.region0;
        if ((location.region1.x > topmostRegion.x && location.region1.y == topmostRegion.y) ||
            (location.region1.x == topmostRegion.x && location.region1.y > topmostRegion.y)) {
            topmostRegion = location.region1;
        } else if ((location.region2.x > topmostRegion.x && location.region2.y == topmostRegion.y) ||
            (location.region2.x == topmostRegion.x && location.region2.y > topmostRegion.y)) {
            topmostRegion = location.region2;
        }

        // Up-left
        result.locations[0].region0 = topmostRegion;
        result.locations[0].region1 = getAdjacentRegion(topmostRegion, DOWN_LEFT);
        result.locations[0].region2 = getAdjacentRegion(topmostRegion, UP_LEFT);

        // Right
        result.locations[1].region0 = topmostRegion;
        result.locations[1].region1 = getAdjacentRegion(topmostRegion, DOWN_RIGHT);
        result.locations[1].region2 = getAdjacentRegion(topmostRegion, DOWN);

        // Down-left
        result.locations[2].region0 = result.locations[1].region2; // DOWN
        result.locations[2].region1 = result.locations[0].region1; // DOWN_LEFT
        result.locations[2].region2 = getAdjacentRegion(result.locations[1].region2, DOWN_LEFT);
    }
    return result;
}
