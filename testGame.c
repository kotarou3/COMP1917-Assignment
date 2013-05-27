#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "Game.h"

#define CYAN STUDENT_BQN
#define PURPLE STUDENT_MMONEY
#define YELLOW STUDENT_MJ
#define RED STUDENT_BPS
#define GREEN STUDENT_MTV
#define BLUE STUDENT_THD

#define TEST_DEGREE_VALUES {YELLOW,PURPLE,CYAN,GREEN,RED,YELLOW,PURPLE,YELLOW,CYAN,GREEN,RED,GREEN,PURPLE,YELLOW,BLUE,CYAN,RED,CYAN,GREEN}
#define TEST_DICE_VALUES {8,10,9,3,5,6,12,6,4,11,3,11,2,9,7,4,5,10,8}

#define false 0
#define true !false
typedef unsigned char bool;

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

// Function declarations
static void runTests(void);

// Setup
static void initAllRegions(void);

// Game creation tests
static void testGameCreation(void);
static void testGameCreationUniversities(Game g);
static void testGameCreationRegions(Game g);

// Gameplay tests
static void testGameplay(void);

// Sub-gameplay tests
static void buildArc(Game g, arc a, int expectedResult);
static void tryBuildArc(Game g, arc a, bool expectedResult);

static void buildCampus(Game g, vertex v, bool isGo8, int expectedResult);
static void tryBuildCampus(Game g, vertex v, bool isGo8, bool expectedResult);

static void doStudentRetrain(Game g, int p, int from, int to, int expectedFromResult, int expectedToResult);
static void tryStudentRetrain(Game g, int from, int to, bool expectedResult);

static void testConstantLegalityActions(Game g);
static void testResources(Game g, int p, int arc, int campus, int go8, int thd, int bps, int bqn, int mj, int mtv, int mmoney, int pub, int patent);

// Helper functions
static Game createTestGame(void);

static arc createArc(region a, region b);
static vertex createVertex(region a, region b, region c);

static region createRegion(int x, int y);
static action createBuildArcAction(arc a);
static action createBuildCampusAction(vertex v, bool isGo8);
static action createRetrainAction(degree from, degree to);

static bool isRegionsEqual(region a, region b);
static bool isVerticesEqual(vertex a, vertex b);
static bool isRegionsAdjacent(region a, region b);

static int absz(int n);

int main(void) {
    runTests();
    printf("All tests passed, you are Awesome!!\n");
    return EXIT_SUCCESS;
}

static void runTests(void) {
    assert(sizeof(testDegreeValues) == NUM_REGIONS * sizeof(testDegreeValues[0]));
    assert(sizeof(testDiceValues) == NUM_REGIONS * sizeof(testDiceValues[0]));
    assert(sizeof(initOrder) == NUM_REGIONS * sizeof(initOrder[0]));
    initAllRegions();

    testGameCreation();
    testGameplay();
}

//////////////////////
// Set up functions //
//////////////////////

static void initAllRegions(void) {
    int r = 0;
    int x = -3;
    while (x <= 3) {
        int y = -3;
        while (y <= 3) {
            if (absz(y + x) <= 3) {
                assert(r < NUM_ALL_REGIONS);
                allRegions[r] = createRegion(x, y);
                r++;
            }
            y++;
        }
        x++;
    }
}

//////////////////////////////////
// Test game creation functions //
//////////////////////////////////

static void testGameCreation(void) {
    Game g = createTestGame();

    // Test basic info
    assert(getTurnNumber(g) == -1);
    assert(getWhoseTurn(g) == UNI_C);
    assert(getMostARCs(g) == UNI_C);
    assert(getMostPublications(g) == UNI_C);
    assert(getKPIpoints(g, UNI_A) == 20);
    assert(getKPIpoints(g, UNI_B) == 20);
    assert(getKPIpoints(g, UNI_C) == 40);

    testGameCreationUniversities(g);
    testGameCreationRegions(g);

    disposeGame(g);
}

static void testGameCreationUniversities(Game g) {
    unsigned int u = 0;
    while (u < NUM_UNIVERSITIES) {
        int playerId = allUnis[u];

        assert(getARCs(g, playerId) == 0);
        assert(getCampuses(g, playerId) == 2);
        assert(getGO8s(g, playerId) == 0);

        assert(getPublications(g, playerId) == 0);
        assert(getIPs(g, playerId) == 0);

        assert(getStudents(g, playerId, STUDENT_THD) == 0);
        assert(getStudents(g, playerId, STUDENT_BPS) == 3);
        assert(getStudents(g, playerId, STUDENT_BQN) == 3);
        assert(getStudents(g, playerId, STUDENT_MJ) == 1);
        assert(getStudents(g, playerId, STUDENT_MTV) == 1);
        assert(getStudents(g, playerId, STUDENT_MMONEY) == 1);

        unsigned int d = 0;
        while (d < NUM_DEGREES) {
            int degreeFromType = allDegrees[d];
            if (degreeFromType != STUDENT_THD) {
                unsigned int d2 = 0;
                while (d2 < NUM_DEGREES) {
                    assert(getExchangeRate(g, playerId, degreeFromType, allDegrees[d2]) == 3);
                    d2++;
                }
            }
            d++;
        }
        u++;
    }
}

static void testGameCreationRegions(Game g) {
    unsigned int r = 0;
    while (r < NUM_ALL_REGIONS) {
        if (absz(allRegions[r].x) == 3 || absz(allRegions[r].y) == 3 || absz(allRegions[r].x + allRegions[r].y) == 3) {
            assert(isSea(g, allRegions[r]));
        } else {
            assert(!isSea(g, allRegions[r]));
        }

        unsigned int r2 = 0;
        while (r2 < NUM_ALL_REGIONS) {
            if (isRegionsAdjacent(allRegions[r2], allRegions[r])) {
                if (!(isSea(g, allRegions[r]) && isSea(g, allRegions[r2]))) {
                    assert(getARC(g, createArc(allRegions[r], allRegions[r2])) == VACANT_ARC);
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

                        assert(getCampus(g, testVertex) == correctVertex);
                    }
                    r3++;
                }
            }
            r2++;
        }
        r++;
    }

    r = 0;
    while (r < NUM_LAND_REGIONS) {
        assert(getDegree(g, initOrder[r]) == testDegreeValues[r]);
        assert(getDiceValue(g, initOrder[r]) == testDiceValues[r]);
        r++;
    }
}

//////////////////////////////
// Test game play functions //
//////////////////////////////

static void testGameplay(void) {
    action passAction;
    action spinoffAction;
    action publicationAction;
    action patentAction;
    passAction.actionCode = PASS;
    spinoffAction.actionCode = START_SPINOFF;
    publicationAction.actionCode = OBTAIN_PUBLICATION;
    patentAction.actionCode = OBTAIN_IP_PATENT;

    // Just check changes in state rather than everything
    Game g = createTestGame();

    // All actions should be illegal before the game starts
    assert(!isLegalAction(g, passAction));
    tryBuildArc(g, createArc(createRegion(0, -2), createRegion(1, -3)), false);
    tryBuildCampus(g, createVertex(createRegion(0, -2), createRegion(0, -1), createRegion(1, -2)), false, false);
    tryBuildCampus(g, createVertex(createRegion(0, -3), createRegion(0, -2), createRegion(1, -3)), true, false);
    assert(!isLegalAction(g, spinoffAction));
    assert(!isLegalAction(g, publicationAction));
    assert(!isLegalAction(g, patentAction));
    assert(!isLegalAction(g, createRetrainAction(STUDENT_BPS, STUDENT_BQN)));

    // Start turn with a dice roll of 11
    throwDice(g, 11);
    assert(getTurnNumber(g) == 0);
    assert(getWhoseTurn(g) == UNI_A);
    assert(getStudents(g, UNI_A, STUDENT_MTV) == 2);
    testConstantLegalityActions(g);

    // Lets try to build a bit
    tryBuildCampus(g, createVertex(createRegion(0, 0), createRegion(1, 0), createRegion(0, 0)), false, false); // Invalid vertex
    tryBuildArc(g, createArc(createRegion(0, 0), createRegion(0, 0)), false); // Invalid edge
    tryBuildCampus(g, createVertex(createRegion(9, 9), createRegion(10, 9), createRegion(10, 8)), false, false); // Non-existent vertex
    tryBuildArc(g, createArc(createRegion(9, 9), createRegion(10, 9)), false); // Non-existent edge
    tryBuildCampus(g, createVertex(createRegion(0, -3), createRegion(0, -2), createRegion(1, -3)), false, false); // Occupied already
    tryBuildArc(g, createArc(createRegion(0, 0), createRegion(1, 0)), false); // Not connected to a campus
    buildArc(g, createArc(createRegion(0, -2), createRegion(1, -3)), ARC_A);
    tryBuildCampus(g, createVertex(createRegion(0, -2), createRegion(0, -1), createRegion(1, -2)), false, false); // Not connected to ARC
    tryBuildCampus(g, createVertex(createRegion(0, -2), createRegion(1, -2), createRegion(1, -3)), false, false); // Can't be adjacent to a campus
    buildArc(g, createArc(createRegion(0, -2), createRegion(1, -2)), ARC_A);
    buildCampus(g, createVertex(createRegion(0, -2), createRegion(0, -1), createRegion(1, -2)), false, CAMPUS_A);

    // We should have now have the following resources
    //                      ARC  Campus  GO8 THD BPS BQN MJ  MTV MMONEY  Pub Patent
    testResources(g, UNI_A, 2,   3,      0,  0,  0,  0,  0,  1,  1,      0,  0);

    // Check KPI points have changed accordingly
    assert(getMostARCs(g) == UNI_A);
    assert(getKPIpoints(g, UNI_A) == 44); // +10 (Most ARCs) +4 (New ARCs) +10 (New campus)
    assert(getKPIpoints(g, UNI_C) == 30); // -10 (Lost most ARCs)

    // Check some other actions for out of resource failure
    tryBuildArc(g, createArc(createRegion(0, -2), createRegion(0, -1)), false);
    tryBuildCampus(g, createVertex(createRegion(0, -2), createRegion(0, -1), createRegion(1, -2)), true, false);
    assert(!isLegalAction(g, spinoffAction));
    assert(!isLegalAction(g, createRetrainAction(STUDENT_BPS, STUDENT_BQN)));

    testConstantLegalityActions(g);

    // ===================================================================================
    // If all tests passed so far, we can assume these work:
    //  - Obtaining resources from a dice roll
    //  - Resource checking for normal campuses and ARCs
    //  - Normal campus and ARC building
    //  - Campus and ARC building checks for existence and validity of target
    //  - Campus and ARC building checks that target is vacant
    // ===================================================================================

    // Next turn with a dice roll of 5
    throwDice(g, 5);
    assert(getTurnNumber(g) == 1);
    assert(getWhoseTurn(g) == UNI_B);
    assert(getStudents(g, UNI_B, STUDENT_BPS) == 4);
    testConstantLegalityActions(g);

    // Build two ARCs and a campus
    buildArc(g, createArc(createRegion(-3, 2), createRegion(-2, 2)), ARC_B);
    buildArc(g, createArc(createRegion(-2, 1), createRegion(-2, 2)), ARC_B);
    buildCampus(g, createVertex(createRegion(-2, 2), createRegion(-2, 1), createRegion(-1, 1)), false, CAMPUS_B);

    // We should have now have the following resources
    //                      ARC  Campus  GO8 THD BPS BQN MJ  MTV MMONEY  Pub Patent
    testResources(g, UNI_B, 2,   3,      0,  0,  1,  0,  0,  0,  1,      0,  0);

    // Check KPI points have changed accordingly
    assert(getMostARCs(g) == UNI_A); // We did not overtake UNI_A even though we are equal, so they are still in lead
    assert(getKPIpoints(g, UNI_A) == 44); // Unchanged from before
    assert(getKPIpoints(g, UNI_B) == 34); // +4 (New ARCs) +10 (New campus)

    testConstantLegalityActions(g);

    // Next turn with a dice roll of 8
    throwDice(g, 8);
    assert(getTurnNumber(g) == 2);
    assert(getWhoseTurn(g) == UNI_C);
    assert(getStudents(g, UNI_C, STUDENT_MJ) == 2);
    assert(getStudents(g, UNI_C, STUDENT_MTV) == 2);

    // Build two ARCs and a campus
    buildArc(g, createArc(createRegion(-2, 0), createRegion(-2, -1)), ARC_C);
    buildArc(g, createArc(createRegion(-2, 0), createRegion(-1, -1)), ARC_C);
    buildCampus(g, createVertex(createRegion(-2, 0), createRegion(-1, 0), createRegion(-1, -1)), false, CAMPUS_C);

    // We should have now have the following resources
    //                      ARC  Campus  GO8 THD BPS BQN MJ  MTV MMONEY  Pub Patent
    testResources(g, UNI_C, 2,   3,      0,  0,  0,  0,  1,  1,  1,      0,  0);

    // Check KPI points have changed accordingly
    assert(getMostARCs(g) == UNI_A);
    assert(getKPIpoints(g, UNI_C) == 44); // +4 (New ARCs) +10 (New campus)

    // Skip ahead a few turns
    throwDice(g, 8);
    throwDice(g, 8);
    throwDice(g, 8);
    throwDice(g, 8);
    throwDice(g, 8);
    throwDice(g, 8);
    assert(getWhoseTurn(g) == UNI_C);

    // ===================================================================================
    // If tests passed so far, we can further assume:
    //  - Turn number increases correctly
    //  - getWhoseTurn() returns correct result
    //  - University keeps position for most ARCs until another university overtakes
    // ===================================================================================

    //                      ARC  Campus  GO8 THD BPS BQN MJ  MTV MMONEY  Pub Patent
    testResources(g, UNI_C, 2,   3,      0,  0,  0,  0,  13,  7,  1,      0,  0);
    doStudentRetrain(g, UNI_C, STUDENT_MJ, STUDENT_BPS, 10, 1);
    doStudentRetrain(g, UNI_C, STUDENT_MJ, STUDENT_BPS, 7, 2);
    doStudentRetrain(g, UNI_C, STUDENT_MJ, STUDENT_BQN, 4, 1);
    doStudentRetrain(g, UNI_C, STUDENT_MJ, STUDENT_BQN, 1, 2);

    // Build a campus at the BPS training centre
    buildArc(g, createArc(createRegion(-2, -1), createRegion(-1, -1)), ARC_C);
    buildCampus(g, createVertex(createRegion(-2, -1), createRegion(-1, -1), createRegion(-1, -2)), false, CAMPUS_C);

    // Check that resources match and UNI_C has regained lead for most ARCs
    //                      ARC  Campus  GO8 THD BPS BQN MJ  MTV MMONEY  Pub Patent
    testResources(g, UNI_C, 3,   4,      0,  0,  0,  0,  0,  6,  1,      0,  0);
    assert(getMostARCs(g) == UNI_C);
    assert(getKPIpoints(g, UNI_A) == 34); // -10 (Lost most ARCs)
    assert(getKPIpoints(g, UNI_C) == 66); // +10 (Most ARCs) +2 (New ARCs) +10 (New campus)

    // Skip ahead a few turns
    throwDice(g, 5);
    throwDice(g, 5);
    throwDice(g, 5);
    assert(getWhoseTurn(g) == UNI_C);

    // Test new student exchange rate
    assert(getExchangeRate(g, UNI_C, STUDENT_BPS, STUDENT_MMONEY) == 2);
    doStudentRetrain(g, UNI_C, STUDENT_BPS, STUDENT_MMONEY, 1, 2);
    tryStudentRetrain(g, STUDENT_BPS, STUDENT_MMONEY, false);

    // Check correct resources for UNI_B and UNI_C
    //                      ARC  Campus  GO8 THD BPS BQN MJ  MTV MMONEY  Pub Patent
    testResources(g, UNI_B, 2,   3,      0,  0,  4,  0,  0,  0,  1,      0,  0);
    testResources(g, UNI_C, 3,   4,      0,  0,  1,  0,  0,  6,  2,      0,  0);

    // ===================================================================================
    // If tests passed so far, we can further assume:
    //  - All campuses adjacent to the same region get their own student boosts
    //  - Student retraining and training centres work correctly
    // ===================================================================================

    // TODO: Check:
    //  - Resource checking for GO8s and spinoffs (Make sure isLegalAction() returns true. Out of resource has been tested)
    //  - Campus to GO8 upgrading (Ensure campus already exists, and there are less than 8 existing GO8s)
    //  - University keeps position for most publications until another university overtakes
    //  - KPI is calculated correctly (GO8s, patents, publication bonus)
    //  - Campus checks for adjacent same-player ARC
    //  - Campus checks for adjacent any-player campus or GO8, and if it exists, fails
    //  - ARC checks for adjacent same-player ARC, campus or GO8

    disposeGame(g);
}

////////////////////////
// Sub-test functions //
////////////////////////

// Checks if building the ARC is valid
static void tryBuildArc(Game g, arc a, bool expectedResult) {
    assert(isLegalAction(g, createBuildArcAction(a)) == expectedResult);
}

// Makes sure the ARC is built and that we can't build again on it
static void buildArc(Game g, arc a, int expectedResult) {
    tryBuildArc(g, a, true);
    makeAction(g, createBuildArcAction(a));
    assert(getARC(g, a) == expectedResult);
    tryBuildArc(g, a, false);
}

// Checks if building the campus is valid
static void tryBuildCampus(Game g, vertex v, bool isGo8, bool expectedResult) {
    action ac = createBuildCampusAction(v, isGo8);
    assert(isLegalAction(g, ac) == expectedResult);
}

// Makes sure the campus is built and that we can't build again on it
static void buildCampus(Game g, vertex v, bool isGo8, int expectedResult) {
    tryBuildCampus(g, v, isGo8, true);
    makeAction(g, createBuildCampusAction(v, isGo8));
    assert(getCampus(g, v) == expectedResult);
}

// Checks if doing a student retrain is valid
static void tryStudentRetrain(Game g, int from, int to, bool expectedResult) {
    assert(isLegalAction(g, createRetrainAction(from, to)) == expectedResult);
}

// Makes sure the student retrain succeeded
static void doStudentRetrain(Game g, int p, int from, int to, int expectedFromResult, int expectedToResult) {
    tryStudentRetrain(g, from, to, true);
    makeAction(g, createRetrainAction(from, to));
    assert(getStudents(g, p, from) == expectedFromResult);
    assert(getStudents(g, p, to) == expectedToResult);
}

static void testConstantLegalityActions(Game g) {
    action passAction;
    action publicationAction;
    action patentAction;
    passAction.actionCode = PASS;
    publicationAction.actionCode = OBTAIN_PUBLICATION;
    patentAction.actionCode = OBTAIN_IP_PATENT;

    // PASS should always be valid move
    assert(isLegalAction(g, passAction));
    // But should not change anything
    int prevTurnNumber = getTurnNumber(g);
    makeAction(g, passAction);
    assert(getTurnNumber(g) == prevTurnNumber);

    // OBTAIN_PUBLICATION and OBTAIN_IP_PATENT should always be illegal
    assert(!isLegalAction(g, publicationAction));
    assert(!isLegalAction(g, patentAction));
}

static void testResources(Game g, int p, int arc, int campus, int go8, int thd, int bps, int bqn, int mj, int mtv, int mmoney, int pub, int patent) {
    assert(getARCs(g, p) == arc);
    assert(getCampuses(g, p) == campus);
    assert(getGO8s(g, p) == go8);
    assert(getStudents(g, p, STUDENT_THD) == thd);
    assert(getStudents(g, p, STUDENT_BPS) == bps);
    assert(getStudents(g, p, STUDENT_BQN) == bqn);
    assert(getStudents(g, p, STUDENT_MJ) == mj);
    assert(getStudents(g, p, STUDENT_MTV) == mtv);
    assert(getStudents(g, p, STUDENT_MMONEY) == mmoney);
    assert(getPublications(g, p) == pub);
    assert(getIPs(g, p) == patent);
}

//////////////////////
// Helper functions //
//////////////////////

static int absz(int n) {
    if (n < 0)
        return -n;
    return n;
}

static Game createTestGame(void) {
    int degrees[] = TEST_DEGREE_VALUES;
    int dice[] = TEST_DICE_VALUES;
    return newGame(degrees, dice);
}

static arc createArc(region a, region b) {
    arc e;
    e.region0 = a;
    e.region1 = b;
    return e;
}

static vertex createVertex(region a, region b, region c) {
    vertex v;
    v.region0 = a;
    v.region1 = b;
    v.region2 = c;
    return v;
}

static region createRegion(int x, int y) {
    region r;
    r.x = x;
    r.y = y;
    return r;
}

static action createBuildArcAction(arc a) {
    action ac;
    ac.actionCode = CREATE_ARC;
    ac.targetARC = a;
    return ac;
}

static action createBuildCampusAction(vertex v, bool isGo8) {
    action ac;
    if (isGo8) {
        ac.actionCode = BUILD_GO8;
    } else {
        ac.actionCode = BUILD_CAMPUS;
    }
    ac.targetVertex = v;
    return ac;
}

static action createRetrainAction(degree from, degree to) {
    action ac;
    ac.actionCode = RETRAIN_STUDENTS;
    ac.retrainFrom = from;
    ac.retrainTo = to;
    return ac;
}

static bool isRegionsEqual(region a, region b) {
    return a.x == b.x && a.y == b.y;
}

static bool isVerticesEqual(vertex a, vertex b) {
    // {a.region0, a.region1, a.region2} ∈ {permutations({b.region0, b.region1, b.region2})}
    return (isRegionsEqual(a.region0, b.region0) && isRegionsEqual(a.region1, b.region1) && isRegionsEqual(a.region2, b.region2)) ||
        (isRegionsEqual(a.region0, b.region0) && isRegionsEqual(a.region1, b.region2) && isRegionsEqual(a.region2, b.region1)) ||
        (isRegionsEqual(a.region0, b.region1) && isRegionsEqual(a.region1, b.region0) && isRegionsEqual(a.region2, b.region2)) ||
        (isRegionsEqual(a.region0, b.region1) && isRegionsEqual(a.region1, b.region2) && isRegionsEqual(a.region2, b.region0)) ||
        (isRegionsEqual(a.region0, b.region2) && isRegionsEqual(a.region1, b.region0) && isRegionsEqual(a.region2, b.region1)) ||
        (isRegionsEqual(a.region0, b.region2) && isRegionsEqual(a.region1, b.region1) && isRegionsEqual(a.region2, b.region0));
}

static bool isRegionsAdjacent(region a, region b) {
    return (a.x == b.x && a.y + 1 == b.y) || // Up
        (a.x == b.x && a.y - 1 == b.y) || // Down
        (a.x - 1 == b.x && a.y + 1 == b.y) || // Up-left
        (a.x - 1 == b.x && a.y == b.y) || // Down-left
        (a.x + 1 == b.x && a.y == b.y) || // Up-right
        (a.x + 1 == b.x && a.y - 1 == b.y); // Down-right
}
