#ifndef MECHANICALTURK_WRAPPER_H
#define MACHANICALTURK_WRAPPER_H

#include "mechanicalTurk-redefines.h"
#include "mechanicalTurk-Resources.h"
#include "mechanicalTurk-Map.h"

typedef struct _MechanicalTurkState {
    PlayerId playerId;
    MechanicalTurkMap map;
    MechanicalTurkResources resources;
} MechanicalTurkState;

void constructMechanicalTurkState(MechanicalTurkState* state, Game* game);
void destroyMechanicalTurkState(MechanicalTurkState* state);

double calculateMechanicalTurkVertexDesirability(const MechanicalTurkState* state, const MechanicalTurkVertex* vertex);

#endif
