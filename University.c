#include <stdlib.h>

#include "Game-wrapper.h"
#include "University.h"

int getARCs(Game* game, PlayerId player) {
    return getOwnedUniversity(game, player)->ownedArcCount;
}

int getCampuses(Game* game, PlayerId player) {
    return getNormalCampusCount(getOwnedUniversity(game, player));
}

int getGO8s(Game* game, PlayerId player) {
    return getGo8CampusCount(getOwnedUniversity(game, player));
}

int getStudents(Game* game, PlayerId player, DegreeType discipline) {
    University* uni = getOwnedUniversity(game, player);
    int students = 0;

    if (discipline == STUDENT_THD) {
        students = uni->studentCount.thd;
    } else if (discipline == STUDENT_BPS) {
        students = uni->studentCount.bps;
    } else if (discipline == STUDENT_BQN) {
        students = uni->studentCount.bqn;
    } else if (discipline == STUDENT_MJ) {
        students = uni->studentCount.mj;
    } else if (discipline == STUDENT_MTV) {
        students = uni->studentCount.mtv;
    } else if (discipline == STUDENT_MMONEY) {
        students = uni->studentCount.mmoney;
    }

    return students;
}

int getExchangeRate(Game* game, PlayerId player, DegreeType from, DegreeType to) {
    return getStudentExchangeRate(getOwnedUniversity(game, player), from, to);
}

int getPublications(Game* game, PlayerId player) {
    return getOwnedUniversity(game, player)->publicationCount;
}

int getIPs(Game* game, PlayerId player) {
    return getOwnedUniversity(game, player)->patentCount;
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

    university->ownedCampusCount = 0;
    university->ownedCampuses = malloc(0);
    university->ownedArcCount = 0;
    university->ownedArcs = malloc(0);
}

void destroyUniversity(University* university) {
    free(university->ownedCampuses);
    free(university->ownedArcs);
}

int getNormalCampusCount(const University* university) {
    int count = 0;
    size_t c = 0;
    while (c < university->ownedCampusCount) {
        if (!university->ownedCampuses[c]->isGo8Campus) {
            ++count;
        }
        ++c;
    }
    return count;
}

int getGo8CampusCount(const University* university) {
    int count = 0;
    size_t c = 0;
    while (c < university->ownedCampusCount) {
        if (university->ownedCampuses[c]->isGo8Campus) {
            ++count;
        }
        ++c;
    }
    return count;
}

int getStudentExchangeRate(const University* university, DegreeType from, DegreeType to) {
    // TODO
}

void buyArc(University* university, Edge* location) {
    // TODO
}

void buyCampus(University* university, Vertex* location, bool isGo8, bool isStarting) {
    // TODO
}
