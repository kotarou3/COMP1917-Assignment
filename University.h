#ifndef UNIVERSITY_H
#define UNIVERSITY_H

#include "Game-redefines.h"
#include "Map.h"

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
