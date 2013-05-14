#include <stdlib.h>
#include <assert.h>

#include "Game-wrapper.h"
#include "University.h"

//defined in game.h but poor name
int getARCs(Game* game, PlayerId player) {
    return getARCCount(game, player);
}

int getARCCount(Game* game, PlayerId player) {
    size_t numArcsOwned = 0;
    size_t e = 0;
    while (e < NUM_EDGES) {
        if ((game->map.edges[e].owner == player) &&
            (game->map.edges[e].isOwned == TRUE)) {
            numArcsOwned++;
        }
        e++;
    }
    
    return (int)numArcsOwned;
}

//defined in game.h but poor name
int getCampuses(Game* game, PlayerId player) {
    return getCampusCount(game, player);
}

int getCampusCount(Game* game, PlayerId player) {
    size_t numCampusesOwned = 0;
    size_t v = 0;
    while (v < NUM_VERTICES) {
        if ((game->map.vertices[v].owner == player) &&
            (game->map.vertices[v].isGo8Campus == FALSE) &&
            (game->map.vertices[v].isOwned == TRUE)) {
            numCampusesOwned++;
        }
        v++;
    }
    
    return (int)numCampusesOwned;
}

Vertex **getPlayerVertexArray(Game* game, PlayerId player) {
    Vertex **vertexArray = malloc(getCampusCount(game, player));
    size_t arrayIndex = 0;
    size_t v = 0;
    while (v < NUM_VERTICES) {
        //Check this function and campus count are consistent
        assert(arrayIndex <= getCampusCount(game, player));
        
        if ((game->map.vertices[v].owner == player) &&
            (game->map.vertices[v].isOwned == TRUE)) {
            vertexArray[arrayIndex] = &game->map.vertices[v];
            arrayIndex++;
        }
        
        v++;
    }
    
    return vertexArray;
}

void destroyPlayerVertexArray(Vertex **vertexArray) {
    free(vertexArray);
}

//defined in game.h but poor name
int getGO8s(Game* game, PlayerId player) {
    return getGO8Count(game, player);
}

int getGO8Count(Game* game, PlayerId player) {
    size_t numGO8sOwned = 0;
    size_t v = 0;
    while (v < NUM_VERTICES) {
        if ((game->map.vertices[v].owner == player) &&
            (game->map.vertices[v].isGo8Campus == TRUE) &&
            (game->map.vertices[v].isOwned == TRUE)) {
            numGO8sOwned++;
        }
        v++;
    }
    
    return (int)numGO8sOwned;
}

int getStudents(Game* game, PlayerId player, DegreeType discipline) {
    University* uni = getOwnedUniversity(game, player, true);
    int students = 0;

    if (discipline == STUDENT_THD) {
        students = (int)uni->studentCount.thd;
    } else if (discipline == STUDENT_BPS) {
        students = (int)uni->studentCount.bps;
    } else if (discipline == STUDENT_BQN) {
        students = (int)uni->studentCount.bqn;
    } else if (discipline == STUDENT_MJ) {
        students = (int)uni->studentCount.mj;
    } else if (discipline == STUDENT_MTV) {
        students = (int)uni->studentCount.mtv;
    } else if (discipline == STUDENT_MMONEY) {
        students = (int)uni->studentCount.mmoney;
    }

    return students;
}

int getExchangeRate(Game* game, PlayerId player, DegreeType from, DegreeType to) {
    size_t exchangeRate = EXCHANGE_RATE_NORMAL;
    
    if ((from == DEGREE_BPS) ||
        (from == DEGREE_BQN) ||
        (from == DEGREE_MJ)  ||
        (from == DEGREE_MTV) ||
        (from == DEGREE_MMONEY)) {
        
        // Check all the owned campuses for the matching training center
        VertexLocation centre1, centre2;
        if (from == DEGREE_BPS) {
            centre1 = TRAINING_CENTRE_BPS_1;
            centre2 = TRAINING_CENTRE_BPS_2;
        } else if (from == DEGREE_BQN) {
            centre1 = TRAINING_CENTRE_BQN_1;
            centre2 = TRAINING_CENTRE_BQN_2;
        } else if (from == DEGREE_MJ) {
            centre1 = TRAINING_CENTRE_MJ_1;
            centre2 = TRAINING_CENTRE_MJ_2;
        } else if (from == DEGREE_MTV) {
            centre1 = TRAINING_CENTRE_MTV_1;
            centre2 = TRAINING_CENTRE_MTV_2;
        } else if (from == DEGREE_MMONEY) {
            centre1 = TRAINING_CENTRE_MMONEY_1;
            centre2 = TRAINING_CENTRE_MMONEY_2;
        }
        
        Vertex **vertexArray = getPlayerVertexArray(game, player);
        size_t c = 0;
        while (c < getCampusCount(game, player)) {
            if (isVerticesEqual(vertexArray[c]->location, centre1) ||
                isVerticesEqual(vertexArray[c]->location, centre2)) {
                exchangeRate = EXCHANGE_RATE_LOW;
            }
            c++;
        }
        destroyPlayerVertexArray(vertexArray);
    }
    
    return (int)exchangeRate;
}

int getPublications(Game* game, PlayerId player) {
    return (int)getOwnedUniversity(game, player, true)->publicationCount;
}

int getIPs(Game* game, PlayerId player) {
    return (int)getOwnedUniversity(game, player, true)->patentCount;
}

void constructUniversity(University* university, PlayerId player) {
    university->playerId = player;

    university->studentCount.thd = START_NUM_THD;
    university->studentCount.bps = START_NUM_BPS;
    university->studentCount.bqn = START_NUM_BQN;
    university->studentCount.mj = START_NUM_MJ;
    university->studentCount.mtv = START_NUM_MTV;
    university->studentCount.mmoney = START_NUM_MMONEY;

    university->publicationCount = START_NUM_PUBLICATIONS;
    university->patentCount = START_NUM_PATENTS;
}

void destroyUniversity(University* university) {
}

bool isPossibleAction(University* university, Map* map, Action action) {
    bool isPossibleAction = FALSE;
    
    if (action.actionCode == BUILD_CAMPUS) {
        isPossibleAction = TRUE;
        
        // Normal campus costs one of BPS, BQN, MJ and MTV, so check we have enough resources
        if (!(university->studentCount.bps >= 1 && university->studentCount.bqn >= 1 &&
            university->studentCount.mj >= 1 && university->studentCount.mtv >= 1))
            isPossibleAction = FALSE;

        // Make sure the vertex exists
        if (getVertex(map, action.targetVertex, false) == NULL) {
            isPossibleAction = FALSE;
        }

        // TODO
        // check all adjacent vertices are VACANT_CAMPUS
        // vertex adjacentVertex1 = vertexAdjToArc(arcAdjToVertex(a.targetVertex, int direction), direction)
        // (navigate to adjacent arcs and then from the arcs to the vertices)
        // Check player has an arc leading to the vertex
    } else if (action.actionCode == BUILD_GO8) {
        // TODO
        // check the vertex exists (regions are adjacent) and is on land
        // check that the player already owns a campus on the vertex
        // check that the player has not reached MAX_NUM_GO8 (need to #define this)
        // check player has enough resources
    } else if (action.actionCode == CREATE_ARC) {
        // TODO
        // check that the arc exists
        // check there isn't an arc already there
        // check that the player owns an adjacent arc
        // check the player has enough resources
    } else if (action.actionCode == START_SPINOFF) {
        // TODO
        // check player has enough of the right resources
    } else if (action.actionCode == RETRAIN_STUDENTS) {
        // TODO
        // check that the player has the right number of students
        // given the exchange rate (use get exchange rate function)
    }
    return isPossibleAction;
}

void doAction(University* university, Map* map, Action action) {
    // TODO
}

void buyArc(University* university, Edge* location) {
    assert(!location->isOwned);

    assert(university->studentCount.bps >= 1 && university->studentCount.bqn >= 1);
    university->studentCount.bps--;
    university->studentCount.bqn--;

    location->isOwned = true;
    location->owner = university->playerId;
}

void buyCampus(University* university, Vertex* location, bool isGo8, bool isStarting) {
    assert(!location->isOwned);

    if (!isStarting) {
        if (isGo8) {
            assert(university->studentCount.mj >= 2 && university->studentCount.mmoney >= 3);
            university->studentCount.mj -= 2;
            university->studentCount.mmoney -=3;
        } else {
            assert(university->studentCount.bps >= 1 && university->studentCount.bqn >= 1 &&
                university->studentCount.mj >= 1 && university->studentCount.mtv >= 1);
            university->studentCount.bps--;
            university->studentCount.bqn--;
            university->studentCount.mj--;
            university->studentCount.mtv--;
        }
    }

    location->isOwned = true;
    location->isGo8Campus = isGo8;
    location->owner = university->playerId;

}
