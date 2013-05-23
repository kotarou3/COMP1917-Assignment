#ifndef MECHANICALTURK_WRAPPER_H
#define MACHANICALTURK_WRAPPER_H

#define INCLUDE_MECHANICALTURK_H
#include "Game-redefines.h"
#include "mechanicalTurk-Resources.h"
#include "mechanicalTurk-Map.h"

typedef struct _MechanicalTurkState {
    PlayerId playerId;
    MechanicalTurkMap map;
    MechanicalTurkResources resources;
} MechanicalTurkState;

void constructMechanicalTurkState(MechanicalTurkState* state, Game* game);
void destroyMechanicalTurkState(MechanicalTurkState* state);

double calculateMechanicalTurkEdgeDesirability(const MechanicalTurkState* state, const MechanicalTurkEdge* edge);
double calculateMechanicalTurkVertexDesirability(const MechanicalTurkState* state, const MechanicalTurkVertex* vertex);

#endif
