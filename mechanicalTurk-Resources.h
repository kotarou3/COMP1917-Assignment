#ifndef MECHANICALTURK_RESOURCES_H
#define MECHANICALTURK_RESOURCES_H

#include "Game-redefines.h"
#include "mechanicalTurk-Map.h"

typedef struct _MechanicalTurkStudentCount {
    size_t thd;
    size_t bps;
    size_t bqn;
    size_t mj;
    size_t mtv;
    size_t mmoney;
} MechanicalTurkStudentCount;

typedef struct _MechanicalTurkResources {
    MechanicalTurkStudentCount studentCount;

    size_t publicationCount;
    size_t patentCount;

    size_t ownedArcCount;
    MechanicalTurkEdge** ownedArcs;

    size_t ownedCampusCount;
    MechanicalTurkVertex** ownedCampuses;
} MechanicalTurkResources;

void constructMechanicalTurkResources(MechanicalTurkResources* resources, Game* game, PlayerId player, MechanicalTurkMap* map);
void destroyMechanicalTurkResources(MechanicalTurkResources* resources);

bool isEnoughMechanicalTurkResourceStudents(MechanicalTurkResources* resources, MechanicalTurkStudentCount cost);

#endif
