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

#define TEST_DEGREE_VALUES {CYAN,PURPLE,YELLOW,PURPLE,YELLOW,RED,GREEN,GREEN,RED,GREEN,CYAN,YELLOW,CYAN,BLUE,YELLOW,PURPLE,GREEN,CYAN,RED}
#define TEST_DICE_VALUES {9,10,8,12,6,5,3,11,3,11,4,6,4,9,9,2,8,10,5}

#define false 0
#define true !false
typedef unsigned char bool;

void _fail_str(bool cond, const char* file, int line, const char* fmt, ...);
void showTestStats(void);
int abs(int n);

Game createTestGame(void);
region createRegion(int x, int y);

#endif
