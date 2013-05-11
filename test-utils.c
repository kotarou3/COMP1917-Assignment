#include <stdio.h>
#include <stdarg.h>

#include "test-utils.h"

int testCount = 0;
int passCount = 0;

void _fail_str(bool cond, const char* file, int line, const char* fmt, ...) {
    ++testCount;
    if (!cond) {
        va_list va;
        va_start(va, fmt);
        fprintf(stderr, "Fail:%s:%d: ", file, line);
        vfprintf(stderr, fmt, va);
        fprintf(stderr, "\n");
    } else {
        ++passCount;
    }
}

void showTestStats(void) {
    fprintf(stderr, "%d/%d tests passed.\n", passCount, testCount);
}

int abs(int n) {
    if (n < 0)
        return -n;
    return n;
}

region createRegion(int x, int y) {
    region r;
    r.x = x;
    r.y = y;
    return r;
}

Game createTestGame(void) {
    int degrees[] = TEST_DEGREE_VALUES;
    int dice[] = TEST_DICE_VALUES;
    return newGame(degrees, dice);
}
