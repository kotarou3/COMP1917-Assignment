#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include "Game.h"

#define CYAN STUDENT_BQN
#define PURPLE STUDENT_MMONEY
#define YELLOW STUDENT_MJ
#define RED STUDENT_BPS
#define GREEN STUDENT_MTV
#define BLUE STUDENT_THD

#define fail(expr) _fail_str(expr, __FILE__, __LINE__, __STRING(expr))
#define fail_str(expr, fmt, ...) _fail_str(expr, __FILE__, __LINE__, fmt, ##__VA_ARGS__)

#define TEST_DEGREE_VALUES {YELLOW,PURPLE,CYAN,GREEN,RED,YELLOW,PURPLE,YELLOW,CYAN,GREEN,RED,GREEN,PURPLE,YELLOW,BLUE,CYAN,RED,CYAN,GREEN}
#define TEST_DICE_VALUES {8,10,9,3,5,6,12,6,4,11,3,11,2,9,7,4,5,10,8}

#define false 0
#define true !false
typedef unsigned char bool;

void _fail_str(bool cond, const char* file, int line, const char* fmt, ...);
void showTestStats(void);
int abs(int n);

Game createTestGame(void);
region createRegion(int x, int y);
arc createArc(region a, region b);
vertex createVertex(region a, region b, region c);

bool isRegionsEqual(region a, region b);
bool isVerticesEqual(vertex a, vertex b);
bool isRegionsAdjacent(region a, region b);

#endif
