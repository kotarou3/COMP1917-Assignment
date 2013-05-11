/*
 *  Game.c
 *  1917 2013 ver 0.0
 *  Add to and change this file as you wish to implement the
 *  interface functions in Game.h
 *
 *  Created by Richard Buckland on 2/5/2013
 *  Licensed under Creative Commons SA-BY-NC 3.0. *
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "Game.h"

//board details
#define NUM_VERTICES 54
#define NUM_ARCS 72
#define MAX_DISTANCE_LAND_REGIONS_FROM_ORIGIN 2
#define MAX_DISTANCE_FROM_ORIGIN (MAX_DISTANCE_LAND_REGIONS_FROM_ORIGIN + 1)
#define BOARD_LENGTH ((2*MAX_DISTANCE_FROM_ORIGIN) + 1)

//directions of arcs and regions
//All directions based on hex grid now
#define NUM_DIRECTIONS 9
#define UP 0
#define UP_RIGHT 2
#define RIGHT 3
#define DOWN_RIGHT 4
#define DOWN 5
#define DOWN_LEFT 6
#define LEFT 7
#define UP_LEFT 8

//Values for sea regions
#define NO_DEGREE -1
#define NO_DICE_VALUE 0

//turn number before first dice roll
#define START_TURN_NUM -1

//Starting number of each resource
#define START_NUM_BPS 3
#define START_NUM_BQN 3
#define START_NUM_MMONEY 1
#define START_NUM_MTV 1
#define START_NUM_MJ 1
#define START_NUM_THD 0

#define START_NUM_PUB 0
#define START_NUM_IP 0

//KPI point values
#define POINTS_PER_ARC 2
#define POINTS_PER_CAMPUS 10
#define POINTS_PER_GO8 20
#define POINTS_PER_IP 10
#define POINTS_FOR_MOST_PUBS 10
#define POINTS_FOR_MOST_ARCS 10

//Trading constants
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

typedef struct _uniResources {
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

typedef struct _regionContents {
    region regionID;
    degree generatedDegree;
    int diceValue; //The number on the hexagon
} regionContents;

typedef struct _vertexContents {
    vertex vertexID;
    int campusType; //ie. CAMPUS_A, GO8_CAMPUS_A, etc.
} vertexContents;

typedef struct _arcContents {
    arc arcID;
    int arcType; //ie. VACANT_ARC, ARC_A, etc.
} arcContents;

// your tutorial class designs this type (not us)
// store in this struct all the things you might want to know about
// the Game so you can write the interface functions in this header
// eg you might want to store the current turn number (so i've put
// it in for you as an example but take it out if you don't want it)
typedef struct _game {
    uniResources universities[NUM_UNIS];
    regionContents regions[BOARD_LENGTH][BOARD_LENGTH];
    arcContents arcs[NUM_ARCS];
    vertexContents vertices[NUM_VERTICES];
    int currentTurn;
} game;

/////////////////////////////
//New game helper functions//
/////////////////////////////

static void initialiseRegions(Game g, int degree[], int dice[]);
static void initialiseVertices(Game g);
static void initialiseVertex(vertexContents *currentVertex, region region0, region region1, region region2);
static void initialiseArcs(Game g);
static void initialiseArc(arcContents *currentArc, region region0, region region1);
static void initialiseUnis(Game g);

////////////////////////////
//General helper functions//
////////////////////////////

//Directions are #defined above
//The following are used for moving around board

//takes UP, UP_RIGHT, DOWN_RIGHT, DOWN, DOWN_LEFT, UP_LEFT only
static region regionAdjToRegion (region currentRegionID, int direction);


//takes UP, UP_RIGHT, DOWN_RIGHT, DOWN, DOWN_LEFT, LEFT only
static arc arcAdjToRegion (region currentRegionID, int direction);

//treats UP, UP_RIGHT and UP_LEFT the same, DOWN, DOWN_LEFT and DOWN_RIGHT the same
//doesn't accept LEFT or RIGHT since arcs are never up/down oriented
static region regionAdjToArc (arc currentArcID, int direction);
static void testRegionAdjToArc (void);

//takes UP_RIGHT, RIGHT, DOWN_RIGHT, DOWN_LEFT, LEFT, UP_LEFT only
static vertex vertexAdjToRegion (region currentRegionID, int direction);

//treats RIGHT, UP_RIGHT and DOWN_RIGHT the same, LEFT, UP_LEFT and DOWN_LEFT the same
//doesn't accept UP or DOWN since arcs are never up/down oriented
static vertex vertexAdjToArc (arc currentArcID, int direction);

//treats UP_RIGHT and RIGHT the same, DOWN_RIGHT and DOWN_LEFT the same, LEFT and UP_LEFT the same
//because two types of vertices and going clockwise these are equivalent
//static arc arcAdjToVertex (vertex currentVertexID, int direction); still needs to be written

//treats UP_RIGHT and RIGHT the same, DOWN_RIGHT and DOWN_LEFT the same, LEFT and UP_LEFT the same
//because only two types of vertices, and going clockwise these positions are equivalent
//static region regionAdjToVertex (vertex currentVertexID, int direction); still needs to be written



//The following are used to get the contents given an id
static vertexContents *getVertexContents (Game g, vertex vertexID);
static regionContents *getRegionContents (Game g, region regionID);
//static arcContents *getArcContents (Game g, vertex vertexID); needs to be finished

static int isLand (region r);


/* **** Functions which change the Game aka SETTERS **** */

// given a Game this function makes it a new Game, resetting everything and
// setting the the type of degree produced by each
// region, and the value on the dice discs in each region.

Game newGame (int degree[], int dice[]) {
    //testRegionAdjToArc();
    
    Game g = malloc(sizeof(struct _game));  // set memory aside for Game
    assert (g!= NULL);                 // check it worked
    
    g->currentTurn = START_TURN_NUM; // initialise currentTurn field
    
    initialiseRegions(g, degree, dice);
    initialiseVertices(g);
    initialiseArcs(g);
    initialiseUnis(g);
    
    //create and set up vertices
    return g;
}

//The function sets the contents of each region based on the image on open learning
void initialiseRegions(Game g, int degree[], int dice[]) {
    //set up regions by looping through array bottom to top, left to right
    //populating each region with the correct dice value and student type
    int x, y;
    
    int degreeDiceIndex = 0;
    //used to keep track of where
    //in degree and dice arrays we're up to
    
    y = -MAX_DISTANCE_FROM_ORIGIN;
    while (y <= MAX_DISTANCE_FROM_ORIGIN) {
        
        x = -MAX_DISTANCE_FROM_ORIGIN;
        while (x <= MAX_DISTANCE_FROM_ORIGIN) {
            regionContents *currentRegion = &(g->regions[x][y]);
            
            //set the regionID of the current region
            //this should not be changed elsewhere
            currentRegion->regionID.x = x;
            currentRegion->regionID.y = y;
            
            if (isLand(currentRegion->regionID)) {
                //Because we travel through land regions in the order
                //that the input arrays correspond to regions, we can just
                //set the contents of each region to the next element in each of
                //those arrays
                
                //Check that we arent going to access out of bounds
                //of either array
                assert(degreeDiceIndex < NUM_REGIONS);
                
                currentRegion->generatedDegree = degree[degreeDiceIndex];
                currentRegion->diceValue = dice[degreeDiceIndex];
                degreeDiceIndex++;
                
            } else {
                currentRegion->generatedDegree = NO_DEGREE;
                currentRegion->diceValue = NO_DICE_VALUE;
            }
            x++;
        }
        
        y++;
    }
    return;
}

//Adds all the vertices to the vertex array of g
//Note there is no inherent order
void initialiseVertices(Game g) {
    int x, y;
    int vertexArrayIndex = 0;
    
    //By looking at diagram on open learning
    //Each region can define 2 vertices
    //one with the region below, and below/right
    //and one with the region right and below/right
    //Except regions in the last column and row
    
    //Loop through regions left to right -1
    //bottom + 1 to top and add the vertices for each region
    
    y = -MAX_DISTANCE_FROM_ORIGIN + 1;
    while (y <= MAX_DISTANCE_FROM_ORIGIN) {
        
        x = -MAX_DISTANCE_FROM_ORIGIN;
        while (x <= MAX_DISTANCE_FROM_ORIGIN - 1) {
            printf("current region %d, %d\n\n", x, y);
            
            vertexContents *currentVertex;
            region region0, region1, region2;
            
            //Add vertex to vertex array in game
            //defined by current region,
            //region below and region down and right
            
            region0 = g->regions[x][y].regionID;
            region1 = regionAdjToRegion(region0, DOWN);
            region2 = regionAdjToRegion(region0, DOWN_RIGHT);
            
            
            //printf("currentVertexIndex %d,\tp:%d,%d\t0: %d,%d\t1: %d,%d\t2: %d,%d\n", vertexArrayIndex, region0.x, region0.y, region1.x, region1.y, region2.x, region2.y);
            
            //If the vertex defined by regions 0, 1, and 2
            //is on/adjacent to land, then add the vertex to the vertex array
            if (isLand(region0) ||
                isLand(region1) ||
                isLand(region2)) {
                assert(vertexArrayIndex < NUM_VERTICES);
                currentVertex = &(g->vertices[vertexArrayIndex]);
                
                initialiseVertex(currentVertex, region0, region1, region2);
                vertexArrayIndex++;
                
            }
            
            
            
            //Add vertex to vertex array in game
            //defined by current region,
            //region to right and region down and right
            
            region0 = g->regions[x][y].regionID;
            region1 = regionAdjToRegion(region0, UP_RIGHT);
            region2 = regionAdjToRegion(region0, DOWN_RIGHT);
            
            //printf("currentVertexIndex %d,\tp:%d,%d\t0: %d,%d\t1: %d,%d\t2: %d,%d\n", vertexArrayIndex, x, y, region0.x, region0.y, region1.x, region1.y, region2.x, region2.y);
            //If the vertex defined by regions 0, 1, and 2
            //is on/adjacent to land, then add the vertex to the vertex array
            if (isLand(region0) ||
                isLand(region1) ||
                isLand(region2)) {
                assert(vertexArrayIndex < NUM_VERTICES);
                currentVertex = &(g->vertices[vertexArrayIndex]);
                
                initialiseVertex(currentVertex, region0, region1, region2);
                vertexArrayIndex++;
                
            }
            
            x++;
        }
        
        y++;
    }
    return;
}

void initialiseVertex(vertexContents *currentVertex, region region0, region region1, region region2) {
    currentVertex->vertexID.region0 = region0;
    currentVertex->vertexID.region1 = region1;
    currentVertex->vertexID.region2 = region2;
    
    currentVertex->campusType = CAMPUS_A;
    return;
}

//Adds all the arcs to the arc array of g
//Note there is no inherent order
void initialiseArcs(Game g) {
    int x, y;
    int arcArrayIndex = 0;
    
    //By looking at diagram on open learning
    //Each region can define 3 arcs
    //one with the region below,
    //one with the region below/right,
    //and one with the region right of the current region
    //Except regions in the last column and row
    
    //Loop through regions left to right -1
    //bottom + 1 to top and add the arcs for each region
    y = -MAX_DISTANCE_FROM_ORIGIN + 1;
    while (y <= MAX_DISTANCE_FROM_ORIGIN) {
        
        x = -MAX_DISTANCE_FROM_ORIGIN;
        while (x <= MAX_DISTANCE_FROM_ORIGIN - 1) {
            arcContents *currentArc;
            region region0, region1;
            
            //Add arc to arc array in game
            //defined by current region,
            //and region below
            
            region0 = g->regions[x][y].regionID;
            region1 = regionAdjToRegion(region0, DOWN);
            
            //If the arc defined by regions 0 and 1
            //is on/adjacent to land, then add the vertex to the vertex array
            if (isLand(region0) ||
                isLand(region1)) {
                
                assert(arcArrayIndex < NUM_ARCS);
                currentArc = &(g->arcs[arcArrayIndex]);
                
                initialiseArc(currentArc, region0, region1);
                arcArrayIndex++;
                
            }
            
            //Add arc to arc array in game
            //defined by current region,
            //and region to right
            
            region0 = g->regions[x][y].regionID;
            region1 = regionAdjToRegion(region0, UP_RIGHT);
            
            //If the arc defined by regions 0 and 1
            //is on/adjacent to land, then add the vertex to the vertex array
            if (isLand(region0) ||
                isLand(region1)) {
                
                assert(arcArrayIndex < NUM_ARCS);
                currentArc = &(g->arcs[arcArrayIndex]);
                
                initialiseArc(currentArc, region0, region1);
                arcArrayIndex++;
                
            }
            
            //Add arc to arc array in game
            //defined by current region,
            //and region below and to right
            
            region0 = g->regions[x][y].regionID;
            region1 = regionAdjToRegion(region0, DOWN_RIGHT);
            
            //If the arc defined by regions 0 and 1
            //is on/adjacent to land, then add the vertex to the vertex array
            if (isLand(region0) ||
                isLand(region1)) {
                
                assert(arcArrayIndex < NUM_ARCS);
                currentArc = &(g->arcs[arcArrayIndex]);
                
                initialiseArc(currentArc, region0, region1);
                arcArrayIndex++;
                
            }
            
            x++;
        }
        
        y++;
    }
    return;
}

void initialiseArc(arcContents *currentArc, region region0, region region1) {
    currentArc->arcID.region0 = region0;
    currentArc->arcID.region1 = region1;
    
    currentArc->arcType = VACANT_ARC;
    return;
}

void initialiseUnis(Game g) {
    uni currentUniID = UNI_A;
    while (currentUniID < NUM_UNIS) {
        uniResources *currentUni = &(g->universities[currentUniID]);
        
        //Set the ID of the uni to the current one
        currentUni->uniID = currentUniID;
        
        //Set the starting number of each resource
        currentUni->numBPS = START_NUM_BPS;
        currentUni->numBQN = START_NUM_BQN;
        currentUni->numMMONEY = START_NUM_MMONEY;
        currentUni->numMTV = START_NUM_MTV;
        currentUni->numMJ = START_NUM_MJ;
        currentUni->numTHD = START_NUM_THD;
        
        //set starting number of publications
        //and patents to 0
        currentUni->numPublications = START_NUM_PUB;
        currentUni->numIPs = START_NUM_IP;
        
        currentUniID++;
    }
    return;
}






// after week 8 we will talk about implementing this. for now
// you can leave it to just do this free
void disposeGame (Game g) {
    assert (g != NULL);
    free (g);
}

// advance the Game to the next turn,
// assuming that the dice has just been rolled and produced diceScore
// the Game starts in turn -1 (we call this state "Terra Nullis") and
// moves to turn 0 as soon as the first dice is thrown.
void throwDice (Game g, int diceScore) {
    g->currentTurn=7;  // julian wrote this line it might need debugging
}

// make the specified action for the current player and update the
// Game state accordingly.
// The function may assume that the action requested is legal.
// START_SPINOFF is not a legal action here
void makeAction (Game g, action a) {
}

/* **** Functions which GET data about the Game aka GETTERS **** */

// true if the region is not one of the land regions of knowledge island
int isSea (Game g, region r) {
    return !isLand(r);
}

// what type of students are produced by the specified land region?
// see degree discipline codes above
degree getDegree (Game g, region r) {
    regionContents *region = getRegionContents(g, r);
    return region->generatedDegree;
}

// what dice value produces students in the specified land region?
// 2..12
int getDiceValue (Game g, region r) {
    return g->regions[r.x][r.y].diceValue;
}

// which university currently has the prestige award for the most ARCs?
// this deemed to be UNI_C at the start of the Game.
uni getMostARCs (Game g) {
    return 0;
}

// which university currently has the prestige award for the most pubs?
// this is deemed to be UNI_C at the start of the Game.
uni getMostPublications (Game g) {
    return 0;
}

// return the current turn number of the Game -1,0,1, ..
int getTurnNumber (Game g) {
    return g->currentTurn;
}

// return the player id of the player whose turn it is
// the result of this function is UNI_C during Terra Nullis
uni getWhoseTurn (Game g) {
    return 0;
}

// return the contents of the given vertex (ie campus code or
// VACANT_VERTEX)
int getCampus (Game g, vertex v) {
    vertexContents *vertex = getVertexContents(g, v);
    return vertex->campusType;
}

// the contents of the given edge (ie ARC code or vacent ARC)
int getARC (Game g, arc a) {
    //arcContents *arc = getArcContents(g, a);
    //return arc->arcType;
    return 0;
}

// returns TRUE if it is legal for the current player
// to make the specified move, FALSE otherwise.
// legal means everything is legal eg when placing a campus consider
// such things as:
//   * is the specified vertex a valid land vertex?
//   * is it vacant?
//   * under the rules of the Game is the current player allowed to
//     place a campus at that vertex?  (eg is it adjacent to one of
//     their ARCs?)
//   * does the player have the 4 specific students required to pay for
//     that campus?
// It is not legal to make any action during Terra Nullis ie
// before the Game has started.
// It is not legal for a player to make the moves OBTAIN_PUBLICATION
// or OBTAIN_IP_PATENT (they can make the move START_SPINOFF)
//
int isLegalAction (Game g, action a) {
    //need vertexIsOnLand(vertex v) function
    //need get vertexContentsFromId(Game g, vertex vId)
    //need to check the contents of adjacent arcs
    //need to check if player has correct amount to pay
    return 0;
}

// --- get data about a specified player ---

// return the number of KPI points the specified player currently has
int getKPIpoints (Game g, uni player) {
    //given the current player, need to use getARCs, getGO8s, getCampuses
    //GetIPs and get publications to determine KPI points
    int kpiPoints;
    
    int arcPoints = getARCs(g, player) * POINTS_PER_ARC;
    int campusPoints = getCampuses(g, player) * POINTS_PER_CAMPUS;
    int go8Points = getGO8s(g, player) * POINTS_PER_GO8;
    int ipPoints = getIPs(g, player) * POINTS_PER_IP;
    
    //If player has most ARCS
    //Give them KPI for that
    int mostArcPoints;
    if (getMostARCs(g) == player) {
        mostArcPoints = POINTS_FOR_MOST_ARCS;
    } else {
        mostArcPoints = 0;
    }
    
    //If player has most publications
    //Give them KPI for that
    int mostPubPoints;
    if (getMostPublications(g) == player) {
        mostPubPoints = POINTS_FOR_MOST_PUBS;
    } else {
        mostPubPoints = 0;
    }
    
    kpiPoints = arcPoints + campusPoints + go8Points +
                ipPoints + mostArcPoints + mostPubPoints;
    
    return kpiPoints;
}

// return the number of ARC grants the specified player currently has
int getARCs (Game g, uni player) {
    return 0;
}

// return the number of GO8 campuses the specified player currently has
int getGO8s (Game g, uni player) {
    return 0;
}

// return the number of normal Campuses the specified player currently has
int getCampuses (Game g, uni player) {
    return 0;
}

// return the number of IP Patents the specified player currently has
int getIPs (Game g, uni player) {
    return 0;
}

// return the number of Publications the specified player currently has
int getPublications (Game g, uni player) {
    return 0;
}

// return the number of students of the specified discipline type
// the specified player currently has
int getStudents (Game g, uni player, degree discipline) {
    return 0;
}

// return how many students of degree type tradeFrom
// the specified player would need to retrain in order to get one
// student of degree type trainTo.  This will depend
// on what retraining centers, if any, they have a campus at.
int getExchangeRate (Game g, uni player,
                     degree tradeFrom, degree tradeTo) {
    //Given the type tradeFrom, look at the two vertices which could be a training centre and check to see if the state of either vertex is a campus or g08 belonging to the player
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

////////////////////////////
//General helper functions//
////////////////////////////


///////////////////////////////
//Move around board functions//
///////////////////////////////

//This function returns the region id (type region) of
//the region in a given direction from the input region

//takes UP, UP_RIGHT, DOWN_RIGHT, DOWN, DOWN_LEFT, UP_LEFT only
region regionAdjToRegion (region currentRegionID, int direction) {
    
    assert((direction == UP) ||
           (direction == UP_RIGHT) ||
           (direction == DOWN_RIGHT) ||
           (direction == DOWN) ||
           (direction == DOWN_LEFT) ||
           (direction == UP_LEFT));
    
    region nextRegionID;

    
    if (direction == DOWN) {
        
        nextRegionID.x = currentRegionID.x;
        nextRegionID.y = currentRegionID.y - 1;
        
    } else if (direction == UP) {
        
        nextRegionID.x = currentRegionID.x;
        nextRegionID.y = currentRegionID.y + 1;
        
    } else if (direction == DOWN_LEFT) {
        
        nextRegionID.x = currentRegionID.x - 1;
        nextRegionID.y = currentRegionID.y;
        
    } else if (direction == UP_RIGHT) {
        
        nextRegionID.x = currentRegionID.x + 1;
        nextRegionID.y = currentRegionID.y;
        
    } else if (direction == UP_LEFT) {
        
        nextRegionID.x = currentRegionID.x - 1;
        nextRegionID.y = currentRegionID.y + 1;
        
    } else if (direction == DOWN_RIGHT) {
        
        nextRegionID.x = currentRegionID.x + 1;
        nextRegionID.y = currentRegionID.y - 1;
        
    } else {
        nextRegionID.x = currentRegionID.x;
        nextRegionID.y = currentRegionID.y;
    }
    
    return nextRegionID;
}

//takes UP, UP_RIGHT, DOWN_RIGHT, DOWN, DOWN_LEFT, LEFT only
arc arcAdjToRegion (region currentRegionID, int direction) {
    
    assert((direction == UP) ||
           (direction == UP_RIGHT) ||
           (direction == DOWN_RIGHT) ||
           (direction == DOWN) ||
           (direction == DOWN_LEFT) ||
           (direction == UP_LEFT));
    
    arc nextArcID;
    
    nextArcID.region0 = currentRegionID;
    nextArcID.region1 = regionAdjToRegion(currentRegionID, direction);
    //need to check that next arc is on land???
    return nextArcID;
}

//treats UP, UP_RIGHT and UP_LEFT the same, DOWN, DOWN_LEFT and DOWN_RIGHT the same
//doesn't accept LEFT or RIGHT since arcs are never up/down oriented
region regionAdjToArc (arc currentArcID, int direction) {
    
    assert((direction != LEFT) && (direction != RIGHT));
    
    //Deal with non UP or DOWN cases
    if ((direction == UP) ||
        (direction == UP_RIGHT) ||
        (direction == UP_LEFT)) {
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


//takes UP_RIGHT, RIGHT, DOWN_RIGHT, DOWN_LEFT, LEFT, UP_LEFT only
vertex vertexAdjToRegion (region currentRegionID, int direction) {
    
    vertex vertexID;
    
    assert((direction == UP_RIGHT) ||
           (direction == RIGHT) ||
           (direction == DOWN_RIGHT) ||
           (direction == DOWN_LEFT) ||
           (direction == LEFT) ||
           (direction == UP_LEFT));
    
    
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

//treats RIGHT, UP_RIGHT and DOWN_RIGHT the same, LEFT, UP_LEFT and DOWN_LEFT the same
//doesn't accept UP or DOWN since arcs are never up/down oriented
vertex vertexAdjToArc (arc currentArcID, int direction) {
    
    assert((direction != UP) && (direction != DOWN));
    
    //Deal with non RIGHT or LEFT cases
    if ((direction == RIGHT) ||
        (direction == UP_RIGHT) ||
        (direction == DOWN_RIGHT) ||
        (direction == UP)) {
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
            topRegion.y--;
            vertexID.region2 = topRegion;
            
        } else {
            
            region bottomRegion = regionAdjToArc(currentArcID, DOWN);
            bottomRegion.x++;
            vertexID.region2 = bottomRegion;
            
        }
        
    } else {
        
        if (currentArcID.region0.y == currentArcID.region1.y) {
            
            region topRegion = regionAdjToArc(currentArcID, DOWN);
            topRegion.y++;
            vertexID.region2 = topRegion;
            
        } else {
            region topRegion = regionAdjToArc(currentArcID, UP);
            topRegion.x--;
            vertexID.region2 = topRegion;
        }
        
    }
    
    return vertexID;

}

//treats UP_RIGHT and RIGHT the same, DOWN_RIGHT and DOWN_LEFT the same, LEFT and UP_LEFT the same
//because two types of vertices and going clockwise these are equivalent
//static arc arcAdjToVertex (vertex currentVertexID, int direction); still needs to be written

//treats UP_RIGHT and RIGHT the same, DOWN_RIGHT and DOWN_LEFT the same, LEFT and UP_LEFT the same
//because only two types of vertices, and going clockwise these positions are equivalent
//static region regionAdjToVertex (vertex currentVertexID, int direction); still needs to be written



/////////////////////////////////
//Get contents from g, given id//
/////////////////////////////////

//returns pointer to the requested regionContents in g
//if it exists else null
regionContents *getRegionContents (Game g, region regionID) {
    regionContents *region = NULL;
    if (isLand(regionID)) {
         region = &(g->regions[regionID.x][regionID.y]);
    }
    return region;
}

//returns pointer to the requested vertexContents in g
//if it exists else null
vertexContents *getVertexContents (Game g, vertex vertexID) {
    int i = 0;
    vertexContents *currentV;
    vertexContents *returnV = NULL;
    
    //Since regions in vertices can be stored in any order,
    //must check all permutations
    
    while (i < NUM_VERTICES) {
        currentV = &(g->vertices[i]);
        
        //check if 1st region matches any of the regions
        if ((currentV->vertexID.region0.x == vertexID.region0.x) &&
             (currentV->vertexID.region0.x == vertexID.region0.y)) {
            
            //Check if second region matches either of the remaining two regions
            if ((currentV->vertexID.region1.x == vertexID.region1.x) &&
                (currentV->vertexID.region1.x == vertexID.region1.y)) {
                
                //check if third region matches remaining region.
                if ((currentV->vertexID.region2.x == vertexID.region2.x) &&
                    (currentV->vertexID.region2.x == vertexID.region2.y)) {
                    //The vertex from the array matches input vertexID
                    returnV = currentV;
                }
                
            } else if ((currentV->vertexID.region1.x == vertexID.region2.x) &&
                       (currentV->vertexID.region1.x == vertexID.region2.y)) {
                
                //check if third region matches remaining region.
                if ((currentV->vertexID.region2.x == vertexID.region1.x) &&
                    (currentV->vertexID.region2.x == vertexID.region1.y)) {
                    //The vertex from the array matches input vertexID
                    returnV = currentV;
                }
                
            }

        } else if ((currentV->vertexID.region0.x == vertexID.region1.x) &&
                   (currentV->vertexID.region0.x == vertexID.region1.y)) {
            
            //Check if second region matches either of the remaining two regions
            if ((currentV->vertexID.region1.x == vertexID.region0.x) &&
                (currentV->vertexID.region1.x == vertexID.region0.y)) {
                
                //check if third region matches remaining region.
                if ((currentV->vertexID.region2.x == vertexID.region2.x) &&
                    (currentV->vertexID.region2.x == vertexID.region2.y)) {
                    //The vertex from the array matches input vertexID
                    returnV = currentV;
                }
                
            } else if ((currentV->vertexID.region1.x == vertexID.region2.x) &&
                       (currentV->vertexID.region1.x == vertexID.region2.y)) {
                
                //check if third region matches remaining region.
                if ((currentV->vertexID.region2.x == vertexID.region0.x) &&
                    (currentV->vertexID.region2.x == vertexID.region0.y)) {
                    //The vertex from the array matches input vertexID
                    returnV = currentV;
                }
                
            }

        } else if ((currentV->vertexID.region0.x == vertexID.region2.x) &&
                   (currentV->vertexID.region0.x == vertexID.region2.y)) {
            
            //Check if second region matches either of the remaining two regions
            if ((currentV->vertexID.region1.x == vertexID.region0.x) &&
                (currentV->vertexID.region1.x == vertexID.region0.y)) {
                
                //check if third region matches remaining region.
                if ((currentV->vertexID.region2.x == vertexID.region1.x) &&
                    (currentV->vertexID.region2.x == vertexID.region1.y)) {
                    //The vertex from the array matches input vertexID
                    returnV = currentV;
                }
                
            } else if ((currentV->vertexID.region1.x == vertexID.region1.x) &&
                       (currentV->vertexID.region1.x == vertexID.region1.y)) {
                
                //check if third region matches remaining region.
                if ((currentV->vertexID.region2.x == vertexID.region0.x) &&
                    (currentV->vertexID.region2.x == vertexID.region0.y)) {
                    //The vertex from the array matches input vertexID
                    returnV = currentV;
                }
                
            }

        }
        
        i++;
    }
    
    return returnV;
    
}

//returns pointer to the requested arcContents in g
//if it exists else null

//Must be finished, see getVertex.
/*
arcContents *getArcContents (Game g, vertex vertexID) {
    int i = 0;
    arcContents *currentV;
    arcContents *returnV = NULL;
    
    while (i < NUM_ARCS) {
        //test all permutations of region0 and region1 here
        
        i++;
    }
    return returnV;
}
*/

//Local version of isSea, doesn't need g
int isLand (region r) {
    int isLand;
    
    int x = r.x;
    int y = r.y;
    
    if ((x >= -MAX_DISTANCE_LAND_REGIONS_FROM_ORIGIN) &&
        (x <= MAX_DISTANCE_LAND_REGIONS_FROM_ORIGIN) &&
        (y >= -MAX_DISTANCE_LAND_REGIONS_FROM_ORIGIN) &&
        (y <= MAX_DISTANCE_LAND_REGIONS_FROM_ORIGIN) &&
        (x + y >= -MAX_DISTANCE_LAND_REGIONS_FROM_ORIGIN) &&
        (x + y <= MAX_DISTANCE_LAND_REGIONS_FROM_ORIGIN)) {
        isLand = TRUE;
    } else {
        isLand = FALSE;
    }
    
    return isLand;
}

void testRegionAdjToArc(void) {
    printf("Testing regionAdjToArc()\n");
    arc currentArc;
    region testingRegion;
    region expectedRegion;
    
    //test 1
    currentArc.region0.x = 0;
    currentArc.region0.y = 0;
    currentArc.region1.x = 0;
    currentArc.region1.y = 1;
    expectedRegion.x = 0;
    expectedRegion.y = 1;
    
    testingRegion = regionAdjToArc(currentArc, UP);
    
    assert(testingRegion.x == expectedRegion.x);
    assert(testingRegion.y == expectedRegion.y);
    
    //test 2
    currentArc.region0.x = 0;
    currentArc.region0.y = 0;
    currentArc.region1.x = 0;
    currentArc.region1.y = 1;
    expectedRegion.x = 0;
    expectedRegion.y = 0;
    
    testingRegion = regionAdjToArc(currentArc, DOWN);
    
    assert(testingRegion.x == expectedRegion.x);
    assert(testingRegion.y == expectedRegion.y);
    
    //test 3
    currentArc.region0.x = 0;
    currentArc.region0.y = 0;
    currentArc.region1.x = 1;
    currentArc.region1.y = 0;
    expectedRegion.x = 1;
    expectedRegion.y = 0;
    
    testingRegion = regionAdjToArc(currentArc, UP);
    
    assert(testingRegion.x == expectedRegion.x);
    assert(testingRegion.y == expectedRegion.y);
    
    //test 4
    currentArc.region0.x = 0;
    currentArc.region0.y = 0;
    currentArc.region1.x = 1;
    currentArc.region1.y = 0;
    expectedRegion.x = 0;
    expectedRegion.y = 0;
    
    testingRegion = regionAdjToArc(currentArc, DOWN);
    
    assert(testingRegion.x == expectedRegion.x);
    assert(testingRegion.y == expectedRegion.y);
    
    //test 5
    currentArc.region0.x = 0;
    currentArc.region0.y = 0;
    currentArc.region1.x = 1;
    currentArc.region1.y = -1;
    expectedRegion.x = 0;
    expectedRegion.y = 0;
    
    testingRegion = regionAdjToArc(currentArc, UP);
    
    assert(testingRegion.x == expectedRegion.x);
    assert(testingRegion.y == expectedRegion.y);
    
    //test 6
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
    
    return;
    
}
