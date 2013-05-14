#ifndef UNIVERSITY_H
#define UNIVERSITY_H

#include "Game-redefines.h"
#include "Map.h"

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

#define CAMPUS_COST ((StudentCount){0, 1, 1, 1, 1, 0})
#define GO8_COST ((StudentCount){0, 0, 0, 2, 0, 3})
#define ARC_COST ((StudentCount){0, 1, 1, 0, 0, 0})

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

int getNormalCampusCount(const University* university);
int getGo8CampusCount(const University* university);
int getStudentExchangeRate(const University* university, DegreeType from, DegreeType to);

bool isPossibleAction(University* university, Map* map, Action action);
void doAction(University* university, Map* map, Action action);

void buyArc(University* university, Edge* location);
void buyCampus(University* university, Vertex* location, bool isGo8, bool isStarting);

void makePurchase (StudentCount *uniStudentCount, StudentCount cost);

#endif
