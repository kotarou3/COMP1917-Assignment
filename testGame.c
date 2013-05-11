#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "Game.h"

#define CYAN STUDENT_BQN
#define PURP STUDENT_MMONEY
#define YELL STUDENT_MJ
#define RED STUDENT_BPS
#define GREE STUDENT_MTV
#define BLUE STUDENT_THD

#define TEST_DISCIPLINES {CYAN,PURP,YELL,PURP,YELL,RED ,GREE,GREE, RED,GREE,CYAN,YELL,CYAN,BLUE,YELL,PURP,GREE,CYAN,RED }
#define TEST_DICE {9,10,8,12,6,5,3,11,3,11,4,6,4,9,9,2,8,10,5}

static void testGetTurnNumberAndWhoseTurn(void);
static void testNewGameGetARCGetCampus(void);
static void testGetKPIPoints(void);
static void testGetExchangeRate(void);

void testGame(void) {
    printf("Testing GameBeta.c - using TEAM_CRAZY's tests\n");
    testNewGameGetARCGetCampus();
    //testGetTurnNumberAndWhoseTurn();
    //testGetKPIPoints();
    //testGetExchangeRate();
    printf("All tests passed!  YOU AREE AAWESOMEEE!!!!!!!! :D \n");
}

void testNewGameGetARCGetCampus(void) {
    printf("***Testing new Game and check contents of the board correct\n");
    printf("makes a new Game without crashing\n");
    degree disciplines[NUM_REGIONS];
    int dice [NUM_REGIONS];

    int regionID = 0;
    while (regionID < NUM_REGIONS) {
        disciplines[regionID] = STUDENT_THD;
        dice[regionID] = 12;
        regionID++;
    }

    Game g = newGame(disciplines, dice);

    // TODO:
    // Test every region is correct
    // Test every vertex is correct
    // Test every ARC is correct
    // Test each player is correct

    assert(getTurnNumber(g) == -1);
    disposeGame(g);
    printf("***Passed!\n");
}

void testGetTurnNumberAndWhoseTurn (void) {
    printf("***Testing get turn number...\n");
    printf("turn number is -1 in terra nullis\n");

    int disciplines[] = TEST_DISCIPLINES;
    int dice[] = TEST_DICE;
    Game g = newGame(disciplines, dice);

    assert(getTurnNumber(g) == -1);
    assert(getWhoseTurn(g) == UNI_C);

    printf("increment turn number\n");
    throwDice(g, 7);

    printf("turn number should now be 0\n");
    assert(getTurnNumber(g) == 0);
    assert(getWhoseTurn(g) == UNI_A);

    printf("increment turn number\n");
    throwDice(g, 2);

    printf("turn number should now be 1\n");
    assert(getTurnNumber(g) == 1);
    assert(getWhoseTurn(g) == UNI_B);

    printf("increment turn number\n");
    throwDice(g, 6);

    printf("turn number should now be 2\n");
    assert(getTurnNumber(g) == 2);
    assert(getWhoseTurn(g) == UNI_C);

    printf("increment turn number\n");
    throwDice(g, 8);

    printf ("turn number should now be 3\n");
    assert(getTurnNumber(g) == 3);
    assert(getWhoseTurn(g) == UNI_A);

    printf("increment turn number\n");
    throwDice(g, 4);

    printf("turn number should now be 4\n");
    assert(getTurnNumber(g) == 4);
    assert(getWhoseTurn(g) == UNI_B);

    disposeGame(g);
    printf("***Passed\n");
}

void testGetKPIPoints(void) {
    // TODO
    // Set up game properly with other functions here once written
    // For the moment will put tests in the game.c file and call from it
}

void testGetExchangeRate(void) {
    // TODO
}
