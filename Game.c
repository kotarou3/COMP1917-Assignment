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
    University* uni = getOwnedUniversity(game, player, true);
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
    return isPossibleAction(getOwnedUniversity(game, getWhoseTurn(game), true), &game->map, action);
}

void makeAction(Game* game, Action action) {
    doAction(getOwnedUniversity(game, getWhoseTurn(game), true), &game->map, action);
}

void throwDice(Game* game, DiceValue diceValue) {
    game->currentTurn++;

    size_t r = 0;
    while (r < NUM_ALL_REGIONS) {
        if (game->map.regions[r].diceValue == diceValue) {
            DegreeType degreeType = game->map.regions[r].generatedDegree;

            size_t d = 0;
            while (d < NUM_DIRECTIONS_VERTEX_FROM_REGION) {
                VertexLocation location = getAdjacentVertexFromRegion(game->map.regions[r].location,
                    validDirections.vertexFromRegion[d]);
                Vertex* vertex = getVertex(&game->map, location, true);

                if (vertex->isOwned) {
                    University* university = getOwnedUniversity(game, vertex->owner, true);

                    if (degreeType == DEGREE_THD) {
                        university->studentCount.thd++;
                    } else if (degreeType == DEGREE_BPS) {
                        university->studentCount.bps++;
                    } else if (degreeType == DEGREE_BQN) {
                        university->studentCount.bqn++;
                    } else if (degreeType == DEGREE_MJ) {
                        university->studentCount.mj++;
                    } else if (degreeType == DEGREE_MTV) {
                        university->studentCount.mtv++;
                    } else if (degreeType == DEGREE_MMONEY) {
                        university->studentCount.mmoney++;
                    }
                }

                d++;
            }
        }
        r++;
    }

    if (diceValue == STUPID_DICE_VALUE_RULE) {
        size_t u = 0;
        while (u < NUM_PLAYERS) {
            University* university = &game->universities[u];

            university->studentCount.thd += university->studentCount.mmoney + university->studentCount.mtv;
            university->studentCount.mmoney = 0;
            university->studentCount.mtv = 0;

            u++;
        }
    }
}

void constructGame(Game* game, DegreeType* regionDegreeTypes, DiceValue* regionDiceValues) {
    game->currentTurn = -1;
    constructMap(&game->map, regionDegreeTypes, regionDiceValues);

    constructUniversity(&game->universities[0], UNI_A);
    constructUniversity(&game->universities[1], UNI_B);
    constructUniversity(&game->universities[2], UNI_C);

    buyCampus(getOwnedUniversity(game, UNI_A, true), getVertex(&game->map, UNI_A_START_CAMPUS_0, true), false, true);
    buyCampus(getOwnedUniversity(game, UNI_A, true), getVertex(&game->map, UNI_A_START_CAMPUS_1, true), false, true);
    buyCampus(getOwnedUniversity(game, UNI_B, true), getVertex(&game->map, UNI_B_START_CAMPUS_0, true), false, true);
    buyCampus(getOwnedUniversity(game, UNI_B, true), getVertex(&game->map, UNI_B_START_CAMPUS_1, true), false, true);
    buyCampus(getOwnedUniversity(game, UNI_C, true), getVertex(&game->map, UNI_C_START_CAMPUS_0, true), false, true);
    buyCampus(getOwnedUniversity(game, UNI_C, true), getVertex(&game->map, UNI_C_START_CAMPUS_1, true), false, true);
}

void destroyGame(Game* game) {
    destroyMap(&game->map);

    size_t u = 0;
    while (u < NUM_PLAYERS) {
        destroyUniversity(&game->universities[u]);
        u++;
    }
}

University* getOwnedUniversity(Game* game, PlayerId player, bool isFatalOnNotFound) {
    size_t u = 0;
    while (u < NUM_PLAYERS) {
        if (game->universities[u].playerId == player) {
            return &game->universities[u];
        }
        u++;
    }
    if (isFatalOnNotFound) {
        assert(!"Invalid player ID");
    }
    return NULL;
}
