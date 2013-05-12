#include <stdlib.h>
#include <assert.h>

#include "Game-wrapper.h"

Game* newGame(DegreeType regionDegreeTypes[], DiceValue regionDiceValues[]) {
    Game* game = malloc(sizeof(Game));
    assert(game != NULL);
    constructGame(game, regionDegreeTypes, regionDiceValues);
    return game;
}

void disposeGame(Game* game) {
    destroyGame(game);
    free(game);
}

int getTurnNumber(Game* game) {
    return game->currentTurn;
}

PlayerId getWhoseTurn(Game* game) {
    return (game->currentTurn + 3) % 3;
}

int getKPIpoints(Game* game, PlayerId player) {
    University* uni = getOwnedUniversity(game, player);
    int kpi = 0;

    kpi += uni->ownedArcCount * KPI_PER_ARC;
    kpi += getNormalCampusCount(uni) * KPI_PER_CAMPUS;
    kpi += getGo8CampusCount(uni) * KPI_PER_GO8;
    kpi += uni->patentCount * KPI_PER_PATENT;

    if (getMostARCs(game) == player) {
        kpi += KPI_FOR_MOST_ARCS;
    }
    if (getMostPublications(game) == player) {
        kpi += KPI_FOR_MOST_PUBS;
    }

    return kpi;
}

PlayerId getMostARCs(Game* game) {
    PlayerId player = NUM_PLAYERS - 1;
    size_t mostArcs = 0;

    size_t u = 0;
    while (u < NUM_PLAYERS) {
        if (game->universities[u].ownedArcCount > mostArcs) {
            player = game->universities[u].playerId;
            mostArcs = game->universities[u].ownedArcCount;
        }
        u++;
    }

    return player;
}

PlayerId getMostPublications(Game* game) {
    PlayerId player = NUM_PLAYERS - 1;
    size_t mostPubs = 0;

    size_t u = 0;
    while (u < NUM_PLAYERS) {
        if (game->universities[u].publicationCount > mostPubs) {
            player = game->universities[u].playerId;
            mostPubs = game->universities[u].publicationCount;
        }
        u++;
    }

    return player;
}

bool isLegalAction(Game* game, Action action) {
    // TODO
}

void makeAction(Game* game, Action action) {
    // TODO
}

void throwDice(Game* game, DiceValue diceValue) {
    // TODO
}

void constructGame(Game* game, DegreeType* regionDegreeTypes, DiceValue* regionDiceValues) {
    game->currentTurn = -1;
    constructMap(&game->map, regionDegreeTypes, regionDiceValues);

    constructUniversity(&game->universities[0], UNI_A);
    constructUniversity(&game->universities[1], UNI_B);
    constructUniversity(&game->universities[2], UNI_C);

    buyCampus(getOwnedUniversity(game, UNI_A), getVertex(&game->map, UNI_A_START_CAMPUS_0), false, true);
    buyCampus(getOwnedUniversity(game, UNI_A), getVertex(&game->map, UNI_A_START_CAMPUS_1), false, true);
    buyCampus(getOwnedUniversity(game, UNI_B), getVertex(&game->map, UNI_B_START_CAMPUS_0), false, true);
    buyCampus(getOwnedUniversity(game, UNI_B), getVertex(&game->map, UNI_B_START_CAMPUS_1), false, true);
    buyCampus(getOwnedUniversity(game, UNI_C), getVertex(&game->map, UNI_C_START_CAMPUS_0), false, true);
    buyCampus(getOwnedUniversity(game, UNI_C), getVertex(&game->map, UNI_C_START_CAMPUS_1), false, true);
}

void destroyGame(Game* game) {
    destroyMap(&game->map);

    size_t u = 0;
    while (u < NUM_PLAYERS) {
        destroyUniversity(&game->universities[u]);
        u++;
    }
}

University* getOwnedUniversity(Game* game, PlayerId player) {
    size_t u = 0;
    while (u < NUM_PLAYERS) {
        if (game->universities[u].playerId == player) {
            return &game->universities[u];
        }
        u++;
    }
    assert(!"Invalid player ID");
}
