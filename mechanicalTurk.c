#include "mechanicalTurk-wrapper.h"

void constructMechanicalTurkState(MechanicalTurkState* state, Game* game) {
    state->playerId = getWhoseTurn(game);
    constructMechanicalTurkMap(&state->map, game);

    size_t v = 0;
    while (v < NUM_VERTICES) {
        state->map.vertices[v].desirability = calculateMechanicalTurkVertexDesirability(state, &state->map.vertices[v]);
        ++v;
    }
}

void destroyMechanicalTurkState(MechanicalTurkState* state) {
    destroyMechanicalTurkMap(&state->map);
}

static int absz(int n) {
    if (n < 0)
        return -n;
    return n;
}
double calculateMechanicalTurkVertexDesirability(const MechanicalTurkState* state, const MechanicalTurkVertex* vertex) {
    // Simple algorithm that just uses the probability of rolling the surrounding regions
    (void)state; // Silence compiler warning

    double pRegions[3] = {0.0, 0.0, 0.0};
    size_t r = 0;
    while (r < 3) {
        if (!vertex->adjacentRegions[r]->isSea) {
            pRegions[r] = (6 - absz(vertex->adjacentRegions[r]->diceValue - 7)) / 36.0;
        }
        ++r;
    }

    return (pRegions[0] + pRegions[1] + pRegions[2]) / 3.0;
}

Action bestMove(Game* game) {
    MechanicalTurkState state;
    constructMechanicalTurkState(&state, game);
    destroyMechanicalTurkState(&state);

    Action move;
    move.actionCode = PASS;
    return move;
}
