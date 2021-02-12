#pragma once

#include "def.h"

#if __cplusplus
extern "C" {
#endif

typedef struct cpblock {
    union {
        void *padd;
        bool  used;
    };
    char load[];
} cpblock;

//the memory pool that manages blocks with certain size.
typedef struct cerpool {
    void *dat;
    int   sin;
    int   sum;
    int   use;
} cerpool;

void cpinit  (cerpool *pool, int sin);
void cpdeinit(cerpool *pool);

void *cpborrow(cerpool *pool);
void  cpreturn(cerpool *pool, void *load);

#if __cplusplus
}
#endif
