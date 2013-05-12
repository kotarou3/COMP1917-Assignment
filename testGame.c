#include <stdlib.h>
#include <assert.h>

#include "test-utils.h"

const int testDegreeValues[] = TEST_DEGREE_VALUES;
const int testDiceValues[] = TEST_DICE_VALUES;
const region initOrder[] = {{-2,0},{-2,1},{-2,2},{-1,-1},{-1,0},{-1,1},{-1,2},{0,-2},{0,-1},{0,0},{0,1},{0,2},{1,-2},{1,-1},{1,0},{1,1},{2,-2},{2,-1},{2,0}};

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
    while (u < NUM_UNIS) {
        fail_str(getARCs(g, u) == 0, "getARCs(g, %d) == 0", u);
        fail_str(getCampuses(g, u) == 2, "getCampuses(g, %d) == 2", u);
        fail_str(getGO8s(g, u) == 0, "getGO8s(g, %d) == 0", u);

        fail_str(getPublications(g, u) == 0, "getPublications(g, %d) == 0", u);
        fail_str(getIPs(g, u) == 0, "getIPs(g, %d) == 0", u);

        fail_str(getStudents(g, u, STUDENT_THD) == 0, "getStudents(g, %d, STUDENT_THD) == 0", u);
        fail_str(getStudents(g, u, STUDENT_BPS) == 3, "getStudents(g, %d, STUDENT_BPS) == 3", u);
        fail_str(getStudents(g, u, STUDENT_BQN) == 3, "getStudents(g, %d, STUDENT_BQN) == 3", u);
        fail_str(getStudents(g, u, STUDENT_MJ) == 1, "getStudents(g, %d, STUDENT_MJ) == 1", u);
        fail_str(getStudents(g, u, STUDENT_MTV) == 1, "getStudents(g, %d, STUDENT_MTV) == 1", u);
        fail_str(getStudents(g, u, STUDENT_MMONEY) == 1, "getStudents(g, %d, STUDENT_MMONEY) == 1", u);

        // TODO: exchange rates
        ++u;
    }

    // Test all regions
    int x = -3;
    while (x <= 3) {
        int y = -3;
        while (y <= 3) {
            if (abs(y + x) <= 3) {
                if (abs(x) == 3 || abs(y) == 3 || abs(x + y) == 3) {
                    fail_str(isSea(g, createRegion(x, y)), "isSea(g, {%d, %d}", x, y);
                } else {
                    fail_str(!isSea(g, createRegion(x, y)), "!isSea(g, {%d, %d}", x, y);
                }
            }
            ++y;
        }
        ++x;
    }

    int r = 0;
    while (r < NUM_REGIONS) {
        fail_str(getDegree(g, initOrder[r]) == testDegreeValues[r], "getDegree(g, {%d, %d}) == %d", initOrder[r].x, initOrder[r].y, testDegreeValues[r]);
        fail_str(getDiceValue(g, initOrder[r]) == testDiceValues[r], "getDiceValue(g, {%d, %d}) == %d", initOrder[r].x, initOrder[r].y, testDiceValues[r]);
        ++r;
    }

    // TODO: Edges and verticies

    disposeGame(g);
}

int main (void) {
    assert(sizeof(testDegreeValues) == NUM_REGIONS * sizeof(testDegreeValues[0]));
    assert(sizeof(testDiceValues) == NUM_REGIONS * sizeof(testDiceValues[0]));
    assert(sizeof(initOrder) == NUM_REGIONS * sizeof(initOrder[0]));
    testGameCreation();

    showTestStats();
    return EXIT_SUCCESS;
}
