#include <stdlib.h>
#include <assert.h>

#include "mechanicalTurk-Resources.h"

void constructMechanicalTurkResources(MechanicalTurkResources* resources, Game* game, PlayerId player, MechanicalTurkMap* map) {
    resources->studentCount.thd = getStudents(game, player, DEGREE_THD);
    resources->studentCount.bps = getStudents(game, player, DEGREE_BPS);
    resources->studentCount.bqn = getStudents(game, player, DEGREE_BQN);
    resources->studentCount.mj = getStudents(game, player, DEGREE_MJ);
    resources->studentCount.mtv = getStudents(game, player, DEGREE_MTV);
    resources->studentCount.mmoney = getStudents(game, player, DEGREE_MMONEY);

    resources->publicationCount = getPublications(game, player);
    resources->patentCount = getIPs(game, player);

    resources->ownedArcCount = getARCs(game, player);
    resources->ownedArcs = malloc(sizeof(resources->ownedArcs[0]) * resources->ownedArcCount);
    size_t a = 0;
    size_t e = 0;
    while (e < NUM_EDGES) {
        if (map->edges[e].isOwned && map->edges[e].owner == player) {
            assert(a < resources->ownedArcCount);
            resources->ownedArcs[a] = &map->edges[e];
            a++;
        }
        e++;
    }
    assert(a == resources->ownedArcCount);

    resources->ownedCampusCount = getCampuses(game, player) + getGO8s(game, player);
    resources->ownedCampuses = malloc(sizeof(resources->ownedCampuses[0]) * resources->ownedCampusCount);
    size_t c = 0;
    size_t v = 0;
    while (v < NUM_VERTICES) {
        if (map->vertices[v].isOwned && map->vertices[v].owner == player) {
            assert(c < resources->ownedCampusCount);
            resources->ownedCampuses[c] = &map->vertices[v];
            c++;
        }
        v++;
    }
    assert(c == resources->ownedCampusCount);
}

void destroyMechanicalTurkResources(MechanicalTurkResources* resources) {
    free(resources->ownedArcs);
    free(resources->ownedCampuses);
}

bool isEnoughMechanicalTurkResourceStudents(MechanicalTurkResources* resources, MechanicalTurkStudentCount cost) {
    return resources->studentCount.thd >= cost.thd &&
        resources->studentCount.bps >= cost.bps &&
        resources->studentCount.bqn >= cost.bqn &&
        resources->studentCount.mj >= cost.mj &&
        resources->studentCount.mtv >= cost.mtv &&
        resources->studentCount.mmoney >= cost.mmoney;
}
