#include "types.h"
#include "defs.h"

#define UCNT_MAX 16 // Maximum number of counters
int ucnt[UCNT_MAX];

void ucnt_init() {
    for (int i = 0; i < UCNT_MAX; i++) {
        ucnt[i] = 0;
    }
}

void ucnt_set(int idx, int val) {
    if (idx < 0 || idx >= UCNT_MAX)
        return;
    ucnt[idx] = val;
}

int ucnt_get(int idx) {
    if (idx < 0 || idx >= UCNT_MAX)
        return 0;
    int val = ucnt[idx];
    return val;
}


