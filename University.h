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

typedef struct {
    PlayerId playerId;

    struct {
        int thd;
        int bps;
        int bqn;
        int mj;
        int mtv;
        int mmoney;
    } studentCount;
    int publicationCount;
    int patentCount;

    Edge** ownedArcs;
    size_t ownedArcCount;

    Vertex** ownedCampuses;
    size_t ownedCampusCount;
} University;

// Game.h functions
int getARCs(Game* game, PlayerId player);
int getCampuses(Game* game, PlayerId player);
int getGO8s(Game* game, PlayerId player);

int getStudents(Game* game, PlayerId player, DegreeType discipline);
int getExchangeRate(Game* game, PlayerId player, DegreeType from, DegreeType to);

int getPublications(Game* game, PlayerId player);
int getIPs(Game* game, PlayerId player);

// Helper functions
void constructUniversity(University* university, PlayerId player);
void destroyUniversity(University* university);

int getStudentExchangeRate(University* university, DegreeType from, DegreeType to);

void buyArc(University* university, Edge* location);
void buyCampus(University* university, Vertex* location, bool isGo8);

#endif
