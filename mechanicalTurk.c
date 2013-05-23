#include <math.h> // For INFINITY
#include <assert.h>

#include "mechanicalTurk-wrapper.h"

void constructMechanicalTurkState(MechanicalTurkState* state, Game* game) {
    state->playerId = getWhoseTurn(game);
    constructMechanicalTurkMap(&state->map, game);
    constructMechanicalTurkResources(&state->resources, game, state->playerId, &state->map);

    size_t v = 0;
    while (v < NUM_VERTICES) {
        state->map.vertices[v].desirability = calculateMechanicalTurkVertexDesirability(state, &state->map.vertices[v]);
        v++;
    }

    size_t e = 0;
    while (e < NUM_EDGES) {
        state->map.edges[e].desirability = calculateMechanicalTurkEdgeDesirability(state, &state->map.edges[e]);
        e++;
    }
}

void destroyMechanicalTurkState(MechanicalTurkState* state) {
    destroyMechanicalTurkMap(&state->map);
    destroyMechanicalTurkResources(&state->resources);
}

double calculateMechanicalTurkEdgeDesirability(const MechanicalTurkState* state, const MechanicalTurkEdge* edge) {
    // Simple algorithm that just averages the desirability of the nearest 6 vertices, with occupied ones ignored
    (void)state; // Silence compiler warning

    double result = 0.0;
    size_t v = 0;
    while (v < edge->adjacentVertexCount) {
        size_t e = 0;
        while (e < edge->adjacentVertices[v]->adjacentEdgeCount) {
            size_t v2 = 0;
            while (v2 < edge->adjacentVertices[v]->adjacentEdges[e]->adjacentVertexCount) {
                MechanicalTurkVertex* currentVertex = edge->adjacentVertices[v]->adjacentEdges[e]->adjacentVertices[v2];
                if (!currentVertex->isOwned) {
                    result += currentVertex->desirability;
                }
                v2++;
            }
            e++;
        }
        v++;
    }
    return result / 6.0 - 1.0; // Subtract 1 to give lower priority to building ARCs than campuses
}

static int absz(int n) {
    if (n < 0)
        return -n;
    return n;
}
double calculateMechanicalTurkVertexDesirability(const MechanicalTurkState* state, const MechanicalTurkVertex* vertex) {
    // Simple algorithm that just averages the probability of rolling the surrounding regions
    (void)state; // Silence compiler warning

    double pRegions[3] = {0.0, 0.0, 0.0};
    size_t r = 0;
    while (r < 3) {
        if (!vertex->adjacentRegions[r]->isSea) {
            pRegions[r] = (6 - absz(vertex->adjacentRegions[r]->diceValue - 7)) / 36.0;
        }
        r++;
    }

    return (pRegions[0] + pRegions[1] + pRegions[2]) / 3.0;
}

static void getBestMoveAtVertex(MechanicalTurkState* state, Game* game, MechanicalTurkVertex* vertex, Action* bestAction, double* bestDesirability) {
    bool isCanBuildArc = isEnoughMechanicalTurkResourceStudents(&state->resources, ARC_COST);
    bool isCanBuildCampus = isEnoughMechanicalTurkResourceStudents(&state->resources, NORMAL_CAMPUS_COST);

    if (isCanBuildCampus) {
        Action action;
        action.actionCode = BUILD_CAMPUS;
        action.targetVertex = vertex->location;
        if (vertex->desirability > *bestDesirability &&
            !vertex->isOwned && isLegalAction(game, action)) {
            *bestAction = action;
            *bestDesirability = vertex->desirability;
        }
    }

    if (isCanBuildArc) {
        size_t e = 0;
        while (e < vertex->adjacentEdgeCount) {
            Action action;
            action.actionCode = CREATE_ARC;
            action.targetARC = vertex->adjacentEdges[e]->location;
            if (vertex->adjacentEdges[e]->desirability > *bestDesirability &&
                !vertex->adjacentEdges[e]->isOwned && isLegalAction(game, action)) {
                *bestAction = action;
                *bestDesirability = vertex->adjacentEdges[e]->desirability;
            }
            e++;
        }
    }
}

Action bestMove(Game* game) {
    MechanicalTurkState state;
    constructMechanicalTurkState(&state, game);

    Action bestAction;
    double bestDesirability = -INFINITY;

    size_t a = 0;
    while (a < state.resources.ownedArcCount) {
        size_t v = 0;
        while (v < state.resources.ownedArcs[a]->adjacentVertexCount) {
            getBestMoveAtVertex(&state, game, state.resources.ownedArcs[a]->adjacentVertices[v], &bestAction, &bestDesirability);
            v++;
        }
        a++;
    }

    // This loop isn't nessasary, except at the start of the game
    size_t c = 0;
    while (c < state.resources.ownedCampusCount) {
        getBestMoveAtVertex(&state, game, state.resources.ownedCampuses[c], &bestAction, &bestDesirability);
        c++;
    }

    if (bestDesirability == -INFINITY) {
        bestAction.actionCode = PASS;
    }

    destroyMechanicalTurkState(&state);
    return bestAction;
}
