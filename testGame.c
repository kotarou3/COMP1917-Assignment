#include <stdlib.h>
#include <assert.h>

#include "test-utils.h"

#define UNI_A_START_CAMPUS_0 ((vertex){{-1, 3}, {0, 2}, {0, 3}})
#define UNI_A_START_CAMPUS_1 ((vertex){{0, -3}, {0, -2}, {1, -3}})
#define UNI_B_START_CAMPUS_0 ((vertex){{-3, 2}, {-3, 3}, {-2, 2}})
#define UNI_B_START_CAMPUS_1 ((vertex){{2, -2}, {3, -3}, {3, -2}})
#define UNI_C_START_CAMPUS_0 ((vertex){{-3, 0}, {-2, -1}, {-2, 0}})
#define UNI_C_START_CAMPUS_1 ((vertex){{2, 0}, {2, 1}, {3, 0}})

#define NUM_UNIVERSITIES (sizeof(allUnis) / sizeof(allUnis[0]))
#define NUM_DEGREES (sizeof(allDegrees) / sizeof(allDegrees[0]))
#define NUM_ALL_REGIONS (NUM_REGIONS + 18)
#define NUM_LAND_REGIONS NUM_REGIONS

static const int allUnis[] = {UNI_A, UNI_B, UNI_C};
static const int allDegrees[] = {STUDENT_THD, STUDENT_BPS, STUDENT_BQN, STUDENT_MJ, STUDENT_MTV, STUDENT_MMONEY};
static region allRegions[NUM_ALL_REGIONS];

static const int testDegreeValues[] = TEST_DEGREE_VALUES;
static const int testDiceValues[] = TEST_DICE_VALUES;
static const region initOrder[] = {{-2,0},{-2,1},{-2,2},{-1,-1},{-1,0},{-1,1},{-1,2},{0,-2},{0,-1},{0,0},{0,1},{0,2},{1,-2},{1,-1},{1,0},{1,1},{2,-2},{2,-1},{2,0}};

static void initAllRegions(void) {
    int r = 0;
    int x = -3;
    while (x <= 3) {
        int y = -3;
        while (y <= 3) {
            if (abs(y + x) <= 3) {
                assert(r < NUM_ALL_REGIONS);
                allRegions[r] = createRegion(x, y);
                ++r;
            }
            ++y;
        }
        ++x;
    }
}

static void testGameCreationUniversities(Game g) {
    unsigned int u = 0;
    while (u < NUM_UNIVERSITIES) {
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

        unsigned int d = 0;
        while (d < NUM_DEGREES) {
            int degreeFromType = allDegrees[d];
            if (degreeFromType != STUDENT_THD) {
                unsigned int d2 = 0;
                while (d2 < NUM_DEGREES) {
                    fail_str(getExchangeRate(g, playerId, degreeFromType, allDegrees[d2]) == 3,
                        "getExchangeRate(g, %d, %d, %d) == 3", playerId, degreeFromType, allDegrees[d2]);
                    ++d2;
                }
            }
            ++d;
        }
        ++u;
    }
}

static void testGameCreationRegions(Game g) {
    unsigned int r = 0;
    while (r < NUM_ALL_REGIONS) {
        if (abs(allRegions[r].x) == 3 || abs(allRegions[r].y) == 3 || abs(allRegions[r].x + allRegions[r].y) == 3) {
            fail_str(isSea(g, allRegions[r]), "isSea(g, {%d, %d})", allRegions[r].x, allRegions[r].y);
        } else {
            fail_str(!isSea(g, allRegions[r]), "!isSea(g, {%d, %d})", allRegions[r].x, allRegions[r].y);
        }

        unsigned int r2 = 0;
        while (r2 < NUM_ALL_REGIONS) {
            if (isRegionsAdjacent(allRegions[r2], allRegions[r])) {
                if (!(isSea(g, allRegions[r]) && isSea(g, allRegions[r2]))) {
                    fail_str(getARC(g, createArc(allRegions[r], allRegions[r2])) == VACANT_ARC,
                        "getARC(g, {{%d, %d}, {%d, %d}}) == VACANT_ARC", allRegions[r].x, allRegions[r].y, allRegions[r2].x, allRegions[r2].y);
                }

                unsigned int r3 = 0;
                while (r3 < NUM_ALL_REGIONS) {
                    if (isRegionsAdjacent(allRegions[r3], allRegions[r]) && isRegionsAdjacent(allRegions[r3], allRegions[r2])) {
                        vertex testVertex = createVertex(allRegions[r], allRegions[r2], allRegions[r3]);

                        int correctVertex = VACANT_VERTEX;
                        if (isVerticesEqual(testVertex, UNI_A_START_CAMPUS_0) || isVerticesEqual(testVertex, UNI_A_START_CAMPUS_1)) {
                            correctVertex = CAMPUS_A;
                        } else if (isVerticesEqual(testVertex, UNI_B_START_CAMPUS_0) || isVerticesEqual(testVertex, UNI_B_START_CAMPUS_1)) {
                            correctVertex = CAMPUS_B;
                        } else if (isVerticesEqual(testVertex, UNI_C_START_CAMPUS_0) || isVerticesEqual(testVertex, UNI_C_START_CAMPUS_1)) {
                            correctVertex = CAMPUS_C;
                        }

                        fail_str(getCampus(g, testVertex) == correctVertex,
                            "getCampus(g, {{%d, %d}, {%d, %d}, {%d, %d}}) == %d",
                            testVertex.region0.x, testVertex.region0.y,
                            testVertex.region1.x, testVertex.region1.y,
                            testVertex.region2.x, testVertex.region2.y, correctVertex);
                    }
                    ++r3;
                }
            }
            ++r2;
        }
        ++r;
    }

    r = 0;
    while (r < NUM_LAND_REGIONS) {
        fail_str(getDegree(g, initOrder[r]) == testDegreeValues[r], "getDegree(g, {%d, %d}) == %d", initOrder[r].x, initOrder[r].y, testDegreeValues[r]);
        fail_str(getDiceValue(g, initOrder[r]) == testDiceValues[r], "getDiceValue(g, {%d, %d}) == %d", initOrder[r].x, initOrder[r].y, testDiceValues[r]);
        ++r;
    }
}

static void testGameCreation(void) {
    Game g = createTestGame();

    // Test basic info
    fail(getTurnNumber(g) == -1);
    fail(getWhoseTurn(g) == UNI_C);
    fail(getMostARCs(g) == UNI_C);
    fail(getMostPublications(g) == UNI_C);
    fail(getKPIpoints(g, UNI_A) == 20);
    fail(getKPIpoints(g, UNI_B) == 20);
    fail(getKPIpoints(g, UNI_C) == 40);

    testGameCreationUniversities(g);
    testGameCreationRegions(g);

    disposeGame(g);
}

bool runTests(void) {
    assert(sizeof(testDegreeValues) == NUM_REGIONS * sizeof(testDegreeValues[0]));
    assert(sizeof(testDiceValues) == NUM_REGIONS * sizeof(testDiceValues[0]));
    assert(sizeof(initOrder) == NUM_REGIONS * sizeof(initOrder[0]));
    initAllRegions();

    testGameCreation();

    return showTestStats();
}

int main(void) {
    if (runTests()) {
        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}
