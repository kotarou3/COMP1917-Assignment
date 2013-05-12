#include <stdlib.h>
#include <assert.h>

#include "test-utils.h"

const int allUnis[] = {UNI_A, UNI_B, UNI_C};
const int uniCount = sizeof(allUnis) / sizeof(allUnis[0]);
const int allDegrees[] = {STUDENT_THD, STUDENT_BPS, STUDENT_BQN, STUDENT_MJ, STUDENT_MTV, STUDENT_MMONEY};
const int degreeCount = sizeof(allDegrees) / sizeof(allDegrees[0]);

const int landRegionCount = NUM_REGIONS;
region allRegions[NUM_REGIONS + 18];
const int regionCount = NUM_REGIONS + 18;

const int testDegreeValues[] = TEST_DEGREE_VALUES;
const int testDiceValues[] = TEST_DICE_VALUES;
const region initOrder[] = {{-2,0},{-2,1},{-2,2},{-1,-1},{-1,0},{-1,1},{-1,2},{0,-2},{0,-1},{0,0},{0,1},{0,2},{1,-2},{1,-1},{1,0},{1,1},{2,-2},{2,-1},{2,0}};

void initAllRegions(void) {
    int r = 0;
    int x = -3;
    while (x <= 3) {
        int y = -3;
        while (y <= 3) {
            if (abs(y + x) <= 3) {
                assert(r < regionCount);
                allRegions[r] = createRegion(x, y);
                ++r;
            }
            ++y;
        }
        ++x;
    }
}

bool testGameCreation() {
    Game g = createTestGame();

    // Test basic info
    fail(getTurnNumber(g) == -1);
    fail(getWhoseTurn(g) == UNI_C);
    fail(getMostARCs(g) == UNI_C);
    fail(getMostPublications(g) == UNI_C);
    fail(getKPIpoints(g, UNI_A) == 20);
    fail(getKPIpoints(g, UNI_B) == 20);
    fail(getKPIpoints(g, UNI_C) == 40);

    // Test universities
    int u = 0;
    while (u < uniCount) {
        int playerId = allUnis[u];

        fail_str(getARCs(g, playerId) == 0, "getARCs(g, %d) == 0", playerId);
        fail_str(getCampuses(g, playerId) == 2, "getCampuses(g, %d) == 2", playerId);
        fail_str(getGO8s(g, playerId) == 0, "getGO8s(g, %d) == 0", playerId);

        fail_str(getPublications(g, playerId) == 0, "getPublications(g, %d) == 0", playerId);
        fail_str(getIPs(g, playerId) == 0, "getIPs(g, %d) == 0", playerId);

        fail_str(getStudents(g, playerId, STUDENT_THD) == 0, "getStudents(g, %d, STUDENT_THD) == 0", playerId);
        fail_str(getStudents(g, playerId, STUDENT_BPS) == 3, "getStudents(g, %d, STUDENT_BPS) == 3", playerId);
        fail_str(getStudents(g, playerId, STUDENT_BQN) == 3, "getStudents(g, %d, STUDENT_BQN) == 3", playerId);
        fail_str(getStudents(g, playerId, STUDENT_MJ) == 1, "getStudents(g, %d, STUDENT_MJ) == 1", playerId);
        fail_str(getStudents(g, playerId, STUDENT_MTV) == 1, "getStudents(g, %d, STUDENT_MTV) == 1", playerId);
        fail_str(getStudents(g, playerId, STUDENT_MMONEY) == 1, "getStudents(g, %d, STUDENT_MMONEY) == 1", playerId);

        int d = 0;
        while (d < degreeCount) {
            int degreeFromType = allDegrees[d];
            if (degreeFromType != STUDENT_THD) {
                int d2 = 0;
                while (d2 < degreeCount) {
                    int degreeToType = allDegrees[d2];

                    int correctExchangeRate = 3;
                    if ((playerId == UNI_A && degreeToType == STUDENT_MTV) ||
                        (playerId == UNI_B && degreeToType == STUDENT_BQN) ||
                        (playerId == UNI_C && degreeToType == STUDENT_BPS))
                        correctExchangeRate = 2;

                    fail_str(getExchangeRate(g, playerId, degreeFromType, degreeToType) == correctExchangeRate,
                        "getExchangeRate(g, %d, %d, %d) == %d", playerId, degreeFromType, degreeToType, correctExchangeRate);

                    ++d2;
                }
            }
            ++d;
        }

        ++u;
    }

    // Test all regions
    int r = 0;
    while (r < regionCount) {
        if (abs(allRegions[r].x) == 3 || abs(allRegions[r].y) == 3 || abs(allRegions[r].x + allRegions[r].y) == 3) {
            fail_str(isSea(g, allRegions[r]), "isSea(g, {%d, %d})", allRegions[r].x, allRegions[r].y);
        } else {
            fail_str(!isSea(g, allRegions[r]), "!isSea(g, {%d, %d})", allRegions[r].x, allRegions[r].y);
        }

        int r2 = 0;
        while (r2 < regionCount) {
            if (isRegionsAdjacent(allRegions[r2], allRegions[r])) {
                if (!(isSea(g, allRegions[r]) && isSea(g, allRegions[r2]))) {
                    fail_str(getARC(g, createArc(allRegions[r], allRegions[r2])) == VACANT_ARC,
                        "getARC(g, {{%d, %d}, {%d, %d}}) == VACANT_ARC", allRegions[r].x, allRegions[r].y, allRegions[r2].x, allRegions[r2].y);
                }

                int r3 = 0;
                while (r3 < regionCount) {
                    if (isRegionsAdjacent(allRegions[r3], allRegions[r]) && isRegionsAdjacent(allRegions[r3], allRegions[r2])) {
                        fail_str(getCampus(g, createVertex(allRegions[r], allRegions[r2], allRegions[r3])) == VACANT_VERTEX,
                            "getCampus(g, {{%d, %d}, {%d, %d}, {%d, %d}}) == VACANT_VERTEX",
                            allRegions[r].x, allRegions[r].y, allRegions[r2].x, allRegions[r2].y, allRegions[r3].x, allRegions[r3].y);
                    }
                    ++r3;
                }
            }
            ++r2;
        }
        ++r;
    }

    r = 0;
    while (r < landRegionCount) {
        fail_str(getDegree(g, initOrder[r]) == testDegreeValues[r], "getDegree(g, {%d, %d}) == %d", initOrder[r].x, initOrder[r].y, testDegreeValues[r]);
        fail_str(getDiceValue(g, initOrder[r]) == testDiceValues[r], "getDiceValue(g, {%d, %d}) == %d", initOrder[r].x, initOrder[r].y, testDiceValues[r]);
        ++r;
    }

    disposeGame(g);
}

int main (void) {
    assert(sizeof(testDegreeValues) == NUM_REGIONS * sizeof(testDegreeValues[0]));
    assert(sizeof(testDiceValues) == NUM_REGIONS * sizeof(testDiceValues[0]));
    assert(sizeof(initOrder) == NUM_REGIONS * sizeof(initOrder[0]));
    initAllRegions();

    testGameCreation();

    showTestStats();
    return EXIT_SUCCESS;
}
