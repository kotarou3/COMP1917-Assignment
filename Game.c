#include <stdlib.h>
#include <assert.h>

#include "Game-wrapper.h"
#include "VertexLocation-utils.h"

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

PlayerId getMostPublications(Game* game) {
    return game->mostPublicationsPlayer;
}

PlayerId getMostARCs(Game* game) {
    return game->mostArcsPlayer;
}

// This function is used to check whether an action which alters
// the current game's state is a valid move or not in accordance
// with a variety of checks including:
//      * the player has enough resources for the desired action
//      * the target location on the map exists
//      * the target location isn't already owned
//      * the target location has no adjacent campus
//      * the adjacent ARC is owned by the player
//
// Most of the checking is done within the University object
// since it should be for a single player only and not the game
// as a whole.
bool isLegalAction(Game* game, Action action) {
    // Do a bit of global validation before passing to player validation
    if (game->currentTurn < 0) {
        return false;
    }

    if (action.actionCode == BUILD_GO8 && game->totalGo8CampusCount >= MAX_GO8_CAMPUSES) {
        return false;
    }

    return isPossibleAction(getOwnedUniversity(game, getWhoseTurn(game), true),
        &game->map, action);
}

void makeAction(Game* game, Action action) {
    University* university = getOwnedUniversity(game, getWhoseTurn(game), true);
    doAction(university, &game->map, action);

    if (action.actionCode == BUILD_GO8) {
        game->totalGo8CampusCount++;
    }

    // Check for updated ARC or publication counts
    if (university->publicationCount > game->mostPublications) {
        game->mostPublications = university->publicationCount;
        game->mostPublicationsPlayer = university->playerId;
    }
    if (university->ownedArcCount > game->mostArcs) {
        game->mostArcs = university->ownedArcCount;
        game->mostArcsPlayer = university->playerId;
    }
}

void throwDice(Game* game, DiceValue diceValue) {
    game->currentTurn++;

    size_t r = 0;
    while (r < NUM_ALL_REGIONS) {
        if (game->map.regions[r].diceValue == diceValue) {
            DegreeType degreeType = game->map.regions[r].generatedDegree;
            SurroundingVerticesFromRegion vertices = getSurroundingVerticesFromRegion(game->map.regions[r].location);

            size_t v = 0;
            while (v < NUM_SURROUNDING_VERTICES_FROM_REGION) {
                Vertex* vertex = getVertex(&game->map, vertices.locations[v], true);
                if (vertex->isOwned) {
                    size_t increaseAmount = 1;
                    if (vertex->isGo8Campus) {
                        increaseAmount = 2;
                    }

                    University* university = getOwnedUniversity(game, vertex->owner, true);
                    if (degreeType == DEGREE_THD) {
                        university->studentCount.thd += increaseAmount;
                    } else if (degreeType == DEGREE_BPS) {
                        university->studentCount.bps += increaseAmount;
                    } else if (degreeType == DEGREE_BQN) {
                        university->studentCount.bqn += increaseAmount;
                    } else if (degreeType == DEGREE_MJ) {
                        university->studentCount.mj += increaseAmount;
                    } else if (degreeType == DEGREE_MTV) {
                        university->studentCount.mtv += increaseAmount;
                    } else if (degreeType == DEGREE_MMONEY) {
                        university->studentCount.mmoney += increaseAmount;
                    }
                }
                v++;
            }
        }
        r++;
    }

    if (diceValue == STUPID_DICE_VALUE_RULE) {
        size_t u = 0;
        while (u < NUM_PLAYERS) {
            University* university = &game->universities[u];

            university->studentCount.thd += university->studentCount.mmoney;
            university->studentCount.thd += university->studentCount.mtv;
            university->studentCount.mmoney = 0;
            university->studentCount.mtv = 0;

            u++;
        }
    }
}

// This function readys the game by stating all initial university
// locations and constructing the map
void constructGame(Game* game, DegreeType* regionDegreeTypes, DiceValue* regionDiceValues) {
    game->currentTurn = -1;
    constructMap(&game->map, regionDegreeTypes, regionDiceValues);

    constructUniversity(&game->universities[0], UNI_A);
    constructUniversity(&game->universities[1], UNI_B);
    constructUniversity(&game->universities[2], UNI_C);

    buyCampus(getOwnedUniversity(game, UNI_A, true),
        getVertex(&game->map, UNI_A_START_CAMPUS_0, true), false, true);
    buyCampus(getOwnedUniversity(game, UNI_A, true),
        getVertex(&game->map, UNI_A_START_CAMPUS_1, true), false, true);
    buyCampus(getOwnedUniversity(game, UNI_B, true),
        getVertex(&game->map, UNI_B_START_CAMPUS_0, true), false, true);
    buyCampus(getOwnedUniversity(game, UNI_B, true),
        getVertex(&game->map, UNI_B_START_CAMPUS_1, true), false, true);
    buyCampus(getOwnedUniversity(game, UNI_C, true),
        getVertex(&game->map, UNI_C_START_CAMPUS_0, true), false, true);
    buyCampus(getOwnedUniversity(game, UNI_C, true),
        getVertex(&game->map, UNI_C_START_CAMPUS_1, true), false, true);

    game->totalGo8CampusCount = 0;
    game->mostPublications = 0;
    game->mostPublicationsPlayer = UNI_C;
    game->mostArcs = 0;
    game->mostArcsPlayer = UNI_C;
}

// Calls the destructors of all the sub-structures to ensure there is no
// memory leak if they allocated any heap memory
void destroyGame(Game* game) {
    destroyMap(&game->map);

    size_t u = 0;
    while (u < NUM_PLAYERS) {
        destroyUniversity(&game->universities[u]);
        u++;
    }
}

// Finds the University structured owned by a player, given the player id
University* getOwnedUniversity(Game* game, PlayerId player, bool isFatalOnNotFound) {
    University* university = NULL;

    size_t u = 0;
    while (u < NUM_PLAYERS) {
        if (game->universities[u].playerId == player) {
            university = &game->universities[u];
        }
        u++;
    }
    if (isFatalOnNotFound && university == NULL) {
        assert(!"Invalid player ID");
    }
    return university;
}
