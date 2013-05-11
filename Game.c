#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "Game.h"

// Board details
#define NUM_VERTICES 54
#define NUM_ARCS 72
#define MAX_DISTANCE_LAND_REGIONS_FROM_ORIGIN 2
#define MAX_DISTANCE_FROM_ORIGIN (MAX_DISTANCE_LAND_REGIONS_FROM_ORIGIN + 1)
#define BOARD_LENGTH ((2*MAX_DISTANCE_FROM_ORIGIN) + 1)

// Directions of arcs and regions
// All directions based on hex grid now
#define NUM_DIRECTIONS 9
#define UP 0
#define UP_RIGHT 2
#define RIGHT 3
#define DOWN_RIGHT 4
#define DOWN 5
#define DOWN_LEFT 6
#define LEFT 7
#define UP_LEFT 8

// Values for sea regions
#define NO_DEGREE -1
#define NO_DICE_VALUE 0

// Turn number before first dice roll
#define START_TURN_NUM -1

// Starting number of each resource
#define START_NUM_BPS 3
#define START_NUM_BQN 3
#define START_NUM_MMONEY 1
#define START_NUM_MTV 1
#define START_NUM_MJ 1
#define START_NUM_THD 0

#define START_NUM_PUB 0
#define START_NUM_IP 0

// KPI point values
#define POINTS_PER_ARC 2
#define POINTS_PER_CAMPUS 10
#define POINTS_PER_GO8 20
#define POINTS_PER_IP 10
#define POINTS_FOR_MOST_PUBS 10
#define POINTS_FOR_MOST_ARCS 10

// Trading constants
#define NORMAL_EXCHANGE_RATE 3
#define LOWER_EXCHANGE_RATE 2

#define MTV_TRAINING_CENTRE_1 {{-2, 3}, {-1, 3}, {-1, 2}}
#define MTV_TRAINING_CENTRE_2 {{0, 2}, {-1, 3}, {-1, 2}}

#define MMONEY_TRAINING_CENTRE_1 {{0, 2}, {1, 2}, {1, 1}}
#define MMONEY_TRAINING_CENTRE_2 {{2, 1}, {1, 2}, {1, 1}}

#define BQN_TRAINING_CENTRE_1 {{3, -1}, {1, 2}, {1, 1}}
#define BQN_TRAINING_CENTRE_2 {{2, -2}, {1, 2}, {1, 1}}

#define MJ_TRAINING_CENTRE_1 {{1, -3}, {1, -2}, {2, -3}}
#define MJ_TRAINING_CENTRE_2 {{2, -2}, {1, -2}, {2, -3}}

#define BPS_TRAINING_CENTRE_1 {{-1, -2}, {-2, -1}, {-1, -1}}
#define BPS_TRAINING_CENTRE_2 {{-2, -1}, {-2, -1}, {-1, -1}}

typedef struct {
    uni uniID;
    int numTHD;
    int numBPS;
    int numBQN;
    int numMJ;
    int numMTV;
    int numMMONEY;
    int numPublications;
    int numIPs;
} uniResources;

typedef struct {
    region regionID; // Should not be modified
    degree generatedDegree;
    int diceValue; // The number on the hexagon
} regionContents;

typedef struct {
    vertex vertexID;
    int campusType; // ie. CAMPUS_A, GO8_CAMPUS_A, etc.
} vertexContents;

typedef struct {
    arc arcID;
    int arcType; // ie. VACANT_ARC, ARC_A, etc.
} arcContents;

typedef struct _game {
    uniResources universities[NUM_UNIS];
    regionContents regions[BOARD_LENGTH][BOARD_LENGTH];
    arcContents arcs[NUM_ARCS];
    vertexContents vertices[NUM_VERTICES];
    int currentTurn;
} game;

// Initialisation functions
static void initialiseRegions(Game g, int* degree, int* dice);
static void initialiseVertices(Game g);
static void initialiseVertex(vertexContents* currentVertex, region region0, region region1, region region2);
static void initialiseArcs(Game g);
static void initialiseArc(arcContents* currentArc, region region0, region region1);
static void initialiseUnis(Game g);

// The following are used for moving around board

// Takes UP, UP_RIGHT, DOWN_RIGHT, DOWN, DOWN_LEFT, UP_LEFT only
static region regionAdjToRegion(region currentRegionID, int direction);

// Takes UP, UP_RIGHT, DOWN_RIGHT, DOWN, DOWN_LEFT, LEFT only
static arc arcAdjToRegion(region currentRegionID, int direction);

// Treats UP, UP_RIGHT and UP_LEFT the same, DOWN, DOWN_LEFT and DOWN_RIGHT the same
// and doesn't accept LEFT or RIGHT since arcs are never up/down oriented
static region regionAdjToArc(arc currentArcID, int direction);
static void testRegionAdjToArc(void);

// Takes UP_RIGHT, RIGHT, DOWN_RIGHT, DOWN_LEFT, LEFT, UP_LEFT only
static vertex vertexAdjToRegion(region currentRegionID, int direction);

// Treats RIGHT, UP_RIGHT and DOWN_RIGHT the same, LEFT, UP_LEFT and DOWN_LEFT the same
// and doesn't accept UP or DOWN since arcs are never up/down oriented
static vertex vertexAdjToArc(arc currentArcID, int direction);

// Treats UP_RIGHT and RIGHT the same, DOWN_RIGHT and DOWN_LEFT the same, LEFT and UP_LEFT the same
// because two types of vertices and going clockwise these are equivalent
static arc arcAdjToVertex(vertex currentVertexID, int direction);

// Treats UP_RIGHT and RIGHT the same, DOWN_RIGHT and DOWN_LEFT the same, LEFT and UP_LEFT the same
// because only two types of vertices, and going clockwise these positions are equivalent
static region regionAdjToVertex(vertex currentVertexID, int direction);

// The following are used to map an ID to its contents
static vertexContents* getVertexContents(Game g, vertex vertexID);
static regionContents* getRegionContents(Game g, region regionID);
static arcContents* getArcContents(Game g, vertex vertexID);

static int isLand(region r);

Game newGame (int degree[], int dice[]) {
    Game g = malloc(sizeof(game));
    assert(g != NULL);

    g->currentTurn = START_TURN_NUM;

    initialiseRegions(g, degree, dice);
    initialiseVertices(g);
    initialiseArcs(g);
    initialiseUnis(g);

    return g;
}

// Sets the contents of each region based on the image on open learning
void initialiseRegions(Game g, int degree[], int dice[]) {
    // Set up regions by looping through array bottom to top, left to right
    // populating each region with the correct dice value and student type
    int regionIndex = 0;
    int y = -MAX_DISTANCE_FROM_ORIGIN;
    while (y <= MAX_DISTANCE_FROM_ORIGIN) {
        int x = -MAX_DISTANCE_FROM_ORIGIN;
        while (x <= MAX_DISTANCE_FROM_ORIGIN) {
            regionContents* currentRegion = &g->regions[x][y];

            currentRegion->regionID.x = x;
            currentRegion->regionID.y = y;

            if (isLand(currentRegion->regionID)) {
                assert(regionIndex < NUM_REGIONS);
                // Because we travel through land regions in the order
                // that the input arrays correspond to regions, we can just
                // set the contents of each region to the next element in each of
                // those arrays

                currentRegion->generatedDegree = degree[regionIndex];
                currentRegion->diceValue = dice[regionIndex];
                ++regionIndex;
            } else {
                currentRegion->generatedDegree = NO_DEGREE;
                currentRegion->diceValue = NO_DICE_VALUE;
            }
            ++x;
        }
        ++y;
    }
}

// Adds all the vertices to the vertex array of g.
// Note there is no inherent order
void initialiseVertices(Game g) {
    // By looking at diagram on open learning
    // each region can define 2 vertices:
    // one with the region below and below/right,
    // and one with the region right and below/right,
    // except regions in the last column and row

    // Loop through regions left to right - 1
    // bottom + 1 to top and add the vertices for each region

    int vertexIndex = 0;
    int y = -MAX_DISTANCE_FROM_ORIGIN + 1;
    while (y <= MAX_DISTANCE_FROM_ORIGIN) {
        int x = -MAX_DISTANCE_FROM_ORIGIN;
        while (x <= MAX_DISTANCE_FROM_ORIGIN - 1) {
            printf("current region %d, %d\n\n", x, y);

            // Add vertex to vertex array in game
            // defined by current region,
            // region below and region down and right
            region region0 = g->regions[x][y].regionID;
            region region1 = regionAdjToRegion(region0, DOWN);
            region region2 = regionAdjToRegion(region0, DOWN_RIGHT);

            // If the vertex defined by regions 0, 1, and 2
            // is on/adjacent to land, then add the vertex to the vertex array
            if (isLand(region0) || isLand(region1) || isLand(region2)) {
                assert(vertexIndex < NUM_VERTICES);
                initialiseVertex(&g->vertices[vertexIndex], region0, region1, region2);
                ++vertexIndex;
            }

            // Do it again for the other vertex
            region1 = regionAdjToRegion(region0, UP_RIGHT);
            if (isLand(region0) || isLand(region1) || isLand(region2)) {
                assert(vertexIndex < NUM_VERTICES);
                initialiseVertex(&g->vertices[vertexIndex], region0, region1, region2);
                ++vertexIndex;
            }
            ++x;
        }
        ++y;
    }
}

void initialiseVertex(vertexContents* currentVertex, region region0, region region1, region region2) {
    currentVertex->vertexID.region0 = region0;
    currentVertex->vertexID.region1 = region1;
    currentVertex->vertexID.region2 = region2;
    currentVertex->campusType = CAMPUS_A;
}

// Adds all the ARCs to the ARC array of g.
// Note there is no inherent order
void initialiseArcs(Game g) {
    // By looking at diagram on open learning
    // each region can define 3 arcs:
    // one with the region below,
    // one with the region below/right,
    // and one with the region right of the current region,
    // Except regions in the last column and row

    // Loop through regions left to right - 1
    // bottom + 1 to top and add the arcs for each region
    int arcIndex = 0;
    int y = -MAX_DISTANCE_FROM_ORIGIN + 1;
    while (y <= MAX_DISTANCE_FROM_ORIGIN) {
        int x = -MAX_DISTANCE_FROM_ORIGIN;
        while (x <= MAX_DISTANCE_FROM_ORIGIN - 1) {
            // Add ARC to ARC array in game
            // defined by current region,
            // and region below
            region region0 = g->regions[x][y].regionID;
            region region1 = regionAdjToRegion(region0, DOWN);

            // If the arc defined by regions 0 and 1
            // is on/adjacent to land, then add the vertex to the vertex array
            if (isLand(region0) || isLand(region1)) {
                assert(arcIndex < NUM_ARCS);
                initialiseArc(&g->arcs[arcIndex], region0, region1);
                ++arcIndex;
            }

            // Repeat two times for the other ARCs
            region1 = regionAdjToRegion(region0, UP_RIGHT);
            if (isLand(region0) || isLand(region1)) {
                assert(arcIndex < NUM_ARCS);
                initialiseArc(&g->arcs[arcIndex], region0, region1);
                ++arcIndex;
            }
            region1 = regionAdjToRegion(region0, DOWN_RIGHT);
            if (isLand(region0) || isLand(region1)) {
                assert(arcIndex < NUM_ARCS);
                initialiseArc(&g->arcs[arcIndex], region0, region1);
                ++arcIndex;
            }
            ++x;
        }
        ++y;
    }
}

void initialiseArc(arcContents* currentArc, region region0, region region1) {
    currentArc->arcID.region0 = region0;
    currentArc->arcID.region1 = region1;
    currentArc->arcType = VACANT_ARC;
}

void initialiseUnis(Game g) {
    uni currentUniID = UNI_A;
    while (currentUniID < NUM_UNIS) {
        uniResources* currentUni = &g->universities[currentUniID];

        currentUni->uniID = currentUniID;
        currentUni->numBPS = START_NUM_BPS;
        currentUni->numBQN = START_NUM_BQN;
        currentUni->numMMONEY = START_NUM_MMONEY;
        currentUni->numMTV = START_NUM_MTV;
        currentUni->numMJ = START_NUM_MJ;
        currentUni->numTHD = START_NUM_THD;
        currentUni->numPublications = START_NUM_PUB;
        currentUni->numIPs = START_NUM_IP;

        ++currentUniID;
    }
}

void disposeGame(Game g) {
    assert(g != NULL);
    free(g);
}

void throwDice(Game g, int diceScore) {
    // TODO
}

void makeAction(Game g, action a) {
    // TODO
}

int isSea(Game g, region r) {
    return !isLand(r);
}

degree getDegree(Game g, region r) {
    return getRegionContents(g, r)->generatedDegree;
}

int getDiceValue(Game g, region r) {
    return g->regions[r.x][r.y].diceValue;
}

uni getMostARCs(Game g) {
    // TODO
}

uni getMostPublications(Game g) {
    // TODO
}

int getTurnNumber(Game g) {
    return g->currentTurn;
}

uni getWhoseTurn(Game g) {
    // TODO
}

int getCampus(Game g, vertex v) {
    return getVertexContents(g, v)->campusType;
}

int getARC(Game g, arc a) {
    // TODO
    //arcContents *arc = getArcContents(g, a);
    //return arc->arcType;
}

int isLegalAction(Game g, action a) {
    // TODO
    // need vertexIsOnLand(vertex v) function
    // need get vertexContentsFromId(Game g, vertex vId)
    // need to check the contents of adjacent arcs
    // need to check if player has correct amount to pay
}

int getKPIpoints(Game g, uni player) {
    // Given the current player, need to use getARCs, getGO8s, getCampuses,
    // getIPs and get publications to determine KPI points
    int kpiPoints = 0;

    kpiPoints += getARCs(g, player) * POINTS_PER_ARC;
    kpiPoints += getCampuses(g, player) * POINTS_PER_CAMPUS;
    kpiPoints += getGO8s(g, player) * POINTS_PER_GO8;
    kpiPoints += getIPs(g, player) * POINTS_PER_IP;

    if (getMostARCs(g) == player) {
        kpiPoints += POINTS_FOR_MOST_ARCS;
    }
    if (getMostPublications(g) == player) {
        kpiPoints += POINTS_FOR_MOST_PUBS;
    }

    return kpiPoints;
}

int getARCs(Game g, uni player) {
    // TODO
}

int getGO8s(Game g, uni player) {
    // TODO
}

int getCampuses(Game g, uni player) {
    // TODO
}

int getIPs(Game g, uni player) {
    // TODO
}

int getPublications(Game g, uni player) {
    // TODO
}

int getStudents(Game g, uni player, degree discipline) {
    // TODO
}

int getExchangeRate(Game g, uni player, degree tradeFrom, degree tradeTo) {
    // TODO
    // Given the type tradeFrom, look at the two vertices which could be a training centre
    // and check to see if the state of either vertex is a campus or g08 belonging to the player
    int exchangeRate = NORMAL_EXCHANGE_RATE;

    /*
    //For each type, there are two possible resource centre vertices
    //Given a specific type, we check to see if
    //The player has a campus or GO8 on either of these
    //Vertices
    if (tradeFrom == STUDENT_BPS) {

        vertex centre1 = getCampus(g, BPS_TRAINING_CENTRE_1);
        vertex centre2 = getCampus(g, BPS_TRAINING_CENTRE_2);

        if (player == UNI_A) &&
            ((vertex1 == CAMPUS_A) ||
             (vertex1 == GO8_CAMPUS_A) ||
             (vertex2 == CAMPUS_A) ||
             (vertex2 == GO8_CAMPUS_A))) {

                exchangeRate = LOWER_EXCHANGE_RATE;
            }
        } else if (player == UNI_B) {

        } else if (player == UNI_C) {

        }
    } else if (tradeFrom == STUDENT_BQN) {

    } else if (tradeFrom == STUDENT_MJ) {

    } else if (tradeFrom == STUDENT_MTV) {

    } else if (tradeFrom == STUDENT_MMONEY) {

    }
    */
    return exchangeRate;
}

static region regionAdjToRegion(region currentRegionID, int direction) {
    assert(direction == UP || direction == UP_RIGHT || direction == DOWN_RIGHT ||
        direction == DOWN || direction == DOWN_LEFT || direction == UP_LEFT);

    region nextRegionID;
    nextRegionID.x = currentRegionID.x;
    nextRegionID.y = currentRegionID.y;

    if (direction == DOWN) {
        --nextRegionID.y;
    } else if (direction == UP) {
        ++nextRegionID.y;
    } else if (direction == DOWN_LEFT) {
        --nextRegionID.x;
    } else if (direction == UP_RIGHT) {
        ++nextRegionID.x;
    } else if (direction == UP_LEFT) {
        --nextRegionID.x;
        ++nextRegionID.y;
    } else if (direction == DOWN_RIGHT) {
        ++nextRegionID.x;
        --nextRegionID.y;
    }

    return nextRegionID;
}

static arc arcAdjToRegion (region currentRegionID, int direction) {
    assert(direction == UP || direction == UP_RIGHT || direction == DOWN_RIGHT ||
        direction == DOWN || direction == DOWN_LEFT || direction == UP_LEFT);

    arc nextArcID;
    nextArcID.region0 = currentRegionID;
    nextArcID.region1 = regionAdjToRegion(currentRegionID, direction);
    // TODO: Need to check that next arc is on land?
    return nextArcID;
}

static region regionAdjToArc (arc currentArcID, int direction) {
    assert(direction == UP || direction == UP_RIGHT || direction == DOWN_RIGHT ||
        direction == DOWN || direction == DOWN_LEFT || direction == UP_LEFT);

    // Deal with non UP or DOWN cases
    if (direction == UP || direction == UP_RIGHT || direction == UP_LEFT) {
        direction = UP;
    } else {
        direction = DOWN;
    }

    region nextRegionID;
    if (direction == UP) {
        if (currentArcID.region1.y < currentArcID.region0.y) {
            nextRegionID = currentArcID.region0;
        } else if (currentArcID.region1.y > currentArcID.region0.y){
            nextRegionID = currentArcID.region1;
        } else if (currentArcID.region1.x < currentArcID.region0.x) {
            nextRegionID = currentArcID.region0;
        } else {
            nextRegionID = currentArcID.region1;
        }
    } else {
        if (currentArcID.region1.y < currentArcID.region0.y) {
            nextRegionID = currentArcID.region1;
        } else if (currentArcID.region1.y > currentArcID.region0.y){
            nextRegionID = currentArcID.region0;
        } else if (currentArcID.region1.x < currentArcID.region0.x) {
            nextRegionID = currentArcID.region1;
        } else {
            nextRegionID = currentArcID.region0;
        }
    }
    return nextRegionID;
}

static vertex vertexAdjToRegion(region currentRegionID, int direction) {
    assert(direction == UP_RIGHT || direction == RIGHT || direction == DOWN_RIGHT ||
        direction == DOWN_LEFT || direction == LEFT || direction == UP_LEFT);

    vertex vertexID;
    vertexID.region0 = currentRegionID;
    if (direction == UP_RIGHT) {
        vertexID.region1 = regionAdjToRegion(currentRegionID, UP);
        vertexID.region2 = regionAdjToRegion(currentRegionID, UP_RIGHT);
    } else if (direction == RIGHT) {
        vertexID.region1 = regionAdjToRegion(currentRegionID, UP_RIGHT);
        vertexID.region2 = regionAdjToRegion(currentRegionID, DOWN_RIGHT);
    } else if (direction == DOWN_RIGHT) {
        vertexID.region1 = regionAdjToRegion(currentRegionID, DOWN_RIGHT);
        vertexID.region2 = regionAdjToRegion(currentRegionID, DOWN);
    } else if (direction == DOWN_LEFT) {
        vertexID.region1 = regionAdjToRegion(currentRegionID, DOWN);
        vertexID.region2 = regionAdjToRegion(currentRegionID, DOWN_LEFT);
    } else if (direction == LEFT) {
        vertexID.region1 = regionAdjToRegion(currentRegionID, DOWN_LEFT);
        vertexID.region2 = regionAdjToRegion(currentRegionID, UP_LEFT);
    } else {
        vertexID.region1 = regionAdjToRegion(currentRegionID, UP_LEFT);
        vertexID.region2 = regionAdjToRegion(currentRegionID, UP);
    }
    return vertexID;
}

static vertex vertexAdjToArc(arc currentArcID, int direction) {
    assert(direction == UP_RIGHT || direction == RIGHT || direction == DOWN_RIGHT ||
        direction == DOWN_LEFT || direction == LEFT || direction == UP_LEFT);

    if (direction == RIGHT || direction == UP_RIGHT || direction == DOWN_RIGHT) {
        direction = RIGHT;
    } else {
        direction = LEFT;
    }

    vertex vertexID;
    vertexID.region0 = currentArcID.region0;
    vertexID.region1 = currentArcID.region1;
    if (direction == RIGHT) {
        if (currentArcID.region0.y == currentArcID.region1.y) {
            region topRegion = regionAdjToArc(currentArcID, UP);
            --topRegion.y;
            vertexID.region2 = topRegion;
        } else {
            region bottomRegion = regionAdjToArc(currentArcID, DOWN);
            ++bottomRegion.x;
            vertexID.region2 = bottomRegion;
        }
    } else {
        if (currentArcID.region0.y == currentArcID.region1.y) {
            region topRegion = regionAdjToArc(currentArcID, DOWN);
            ++topRegion.y;
            vertexID.region2 = topRegion;
        } else {
            region topRegion = regionAdjToArc(currentArcID, UP);
            --topRegion.x;
            vertexID.region2 = topRegion;
        }
    }
    return vertexID;

}

static arc arcAdjToVertex(vertex currentVertexID, int direction) {
    // TODO
}

static region regionAdjToVertex(vertex currentVertexID, int direction) {
    // TODO
}

regionContents* getRegionContents(Game g, region regionID) {
    if (isLand(regionID)) {
         return &g->regions[regionID.x][regionID.y];
    }
    return NULL;
}

vertexContents* getVertexContents(Game g, vertex vertexID) {
    // Since regions in vertices can be stored in any order,
    // we must check all permutations
    vertexContents* returnV = NULL;
    int vertexIndex = 0;
    while (vertexIndex < NUM_VERTICES) {
        vertexContents* currentV = &g->vertices[vertexIndex];

        // Check if 1st region matches any of the regions
        if (currentV->vertexID.region0.x == vertexID.region0.x &&
            currentV->vertexID.region0.x == vertexID.region0.y) {
            // Check if second region matches either of the remaining two regions
            if (currentV->vertexID.region1.x == vertexID.region1.x &&
                currentV->vertexID.region1.x == vertexID.region1.y) {
                // Check if third region matches remaining region.
                if (currentV->vertexID.region2.x == vertexID.region2.x &&
                    currentV->vertexID.region2.x == vertexID.region2.y) {
                    //The vertex from the array matches input vertexID
                    returnV = currentV;
                }
            } else if (currentV->vertexID.region1.x == vertexID.region2.x &&
                       currentV->vertexID.region1.x == vertexID.region2.y) {
                // Check if third region matches remaining region.
                if (currentV->vertexID.region2.x == vertexID.region1.x &&
                    currentV->vertexID.region2.x == vertexID.region1.y) {
                    //The vertex from the array matches input vertexID
                    returnV = currentV;
                }
            }
        } else if (currentV->vertexID.region0.x == vertexID.region1.x &&
                currentV->vertexID.region0.x == vertexID.region1.y) {
            // Check if second region matches either of the remaining two regions
            if (currentV->vertexID.region1.x == vertexID.region0.x &&
                currentV->vertexID.region1.x == vertexID.region0.y) {
                // Check if third region matches remaining region.
                if (currentV->vertexID.region2.x == vertexID.region2.x &&
                    currentV->vertexID.region2.x == vertexID.region2.y) {
                    // The vertex from the array matches input vertexID
                    returnV = currentV;
                }
            } else if (currentV->vertexID.region1.x == vertexID.region2.x &&
                currentV->vertexID.region1.x == vertexID.region2.y) {
                // Check if third region matches remaining region.
                if (currentV->vertexID.region2.x == vertexID.region0.x &&
                    currentV->vertexID.region2.x == vertexID.region0.y) {
                    // The vertex from the array matches input vertexID
                    returnV = currentV;
                }
            }
        } else if (currentV->vertexID.region0.x == vertexID.region2.x &&
            currentV->vertexID.region0.x == vertexID.region2.y) {
            // Check if second region matches either of the remaining two regions
            if (currentV->vertexID.region1.x == vertexID.region0.x &&
                currentV->vertexID.region1.x == vertexID.region0.y) {
                // Check if third region matches remaining region.
                if (currentV->vertexID.region2.x == vertexID.region1.x &&
                    currentV->vertexID.region2.x == vertexID.region1.y) {
                    // The vertex from the array matches input vertexID
                    returnV = currentV;
                }
            } else if (currentV->vertexID.region1.x == vertexID.region1.x &&
                currentV->vertexID.region1.x == vertexID.region1.y) {
                // Check if third region matches remaining region.
                if (currentV->vertexID.region2.x == vertexID.region0.x &&
                    currentV->vertexID.region2.x == vertexID.region0.y) {
                    // The vertex from the array matches input vertexID
                    returnV = currentV;
                }
            }
        }
        ++vertexIndex;
    }
    return returnV;
}

arcContents* getArcContents(Game g, vertex vertexID) {
    // TODO
    /*int i = 0;
    arcContents *currentV;
    arcContents *returnV = NULL;

    while (i < NUM_ARCS) {
        //test all permutations of region0 and region1 here

        i++;
    }
    return returnV;*/
}

int isLand(region r) {
    if (r.x >= -MAX_DISTANCE_LAND_REGIONS_FROM_ORIGIN &&
        r.x <= MAX_DISTANCE_LAND_REGIONS_FROM_ORIGIN &&
        r.y >= -MAX_DISTANCE_LAND_REGIONS_FROM_ORIGIN &&
        r.y <= MAX_DISTANCE_LAND_REGIONS_FROM_ORIGIN &&
        r.x + r.y >= -MAX_DISTANCE_LAND_REGIONS_FROM_ORIGIN &&
        r.x + r.y <= MAX_DISTANCE_LAND_REGIONS_FROM_ORIGIN) {
        return TRUE;
    }
    return FALSE;
}

void testRegionAdjToArc(void) {
    printf("Testing regionAdjToArc()\n");
    arc currentArc;
    region testingRegion;
    region expectedRegion;

    // Test 1
    currentArc.region0.x = 0;
    currentArc.region0.y = 0;
    currentArc.region1.x = 0;
    currentArc.region1.y = 1;
    expectedRegion.x = 0;
    expectedRegion.y = 1;

    testingRegion = regionAdjToArc(currentArc, UP);
    assert(testingRegion.x == expectedRegion.x);
    assert(testingRegion.y == expectedRegion.y);

    // Test 2
    currentArc.region0.x = 0;
    currentArc.region0.y = 0;
    currentArc.region1.x = 0;
    currentArc.region1.y = 1;
    expectedRegion.x = 0;
    expectedRegion.y = 0;

    testingRegion = regionAdjToArc(currentArc, DOWN);
    assert(testingRegion.x == expectedRegion.x);
    assert(testingRegion.y == expectedRegion.y);

    // Test 3
    currentArc.region0.x = 0;
    currentArc.region0.y = 0;
    currentArc.region1.x = 1;
    currentArc.region1.y = 0;
    expectedRegion.x = 1;
    expectedRegion.y = 0;

    testingRegion = regionAdjToArc(currentArc, UP);
    assert(testingRegion.x == expectedRegion.x);
    assert(testingRegion.y == expectedRegion.y);

    // Test 4
    currentArc.region0.x = 0;
    currentArc.region0.y = 0;
    currentArc.region1.x = 1;
    currentArc.region1.y = 0;
    expectedRegion.x = 0;
    expectedRegion.y = 0;

    testingRegion = regionAdjToArc(currentArc, DOWN);
    assert(testingRegion.x == expectedRegion.x);
    assert(testingRegion.y == expectedRegion.y);

    // Test 5
    currentArc.region0.x = 0;
    currentArc.region0.y = 0;
    currentArc.region1.x = 1;
    currentArc.region1.y = -1;
    expectedRegion.x = 0;
    expectedRegion.y = 0;

    testingRegion = regionAdjToArc(currentArc, UP);
    assert(testingRegion.x == expectedRegion.x);
    assert(testingRegion.y == expectedRegion.y);

    // Test 6
    currentArc.region0.x = 0;
    currentArc.region0.y = 0;
    currentArc.region1.x = 1;
    currentArc.region1.y = -1;
    expectedRegion.x = 1;
    expectedRegion.y = -1;

    testingRegion = regionAdjToArc(currentArc, DOWN);
    assert(testingRegion.x == expectedRegion.x);
    assert(testingRegion.y == expectedRegion.y);

    printf("All tests passed, YOU ARE AWESOME!!\n\n");
}
