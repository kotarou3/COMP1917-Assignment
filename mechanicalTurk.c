#include "mechanicalTurk-redefines.h"
#include "mechanicalTurk-Map.h"

typedef struct _MechanicalTurkState {
    PlayerId playerId;
    MechanicalTurkMap map;
} MechanicalTurkState;

void constructMechanicalTurkState(MechanicalTurkState* state, Game* game) {
    state->playerId = getWhoseTurn(game);
    constructMechanicalTurkMap(&state->map, game);
}

void destroyMechanicalTurkState(MechanicalTurkState* state) {
    destroyMechanicalTurkMap(&state->map);
}

Action bestMove(Game* game) {
    MechanicalTurkState state;
    constructMechanicalTurkState(&state, game);
    destroyMechanicalTurkState(&state);

    Action move;
    move.actionCode = PASS;
    return move;
}
