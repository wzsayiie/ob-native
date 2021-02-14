#pragma once

#include "def.h"

#if __cplusplus
extern "C" {
#endif

typedef struct cpchip  cpchip ;
typedef struct cpboard cpboard;
typedef struct cerpool cerpool;

struct cpchip {
    cpboard *board;
    char     carry[0];
};

struct cpboard {
    cpboard *prev;
    cpboard *next;
    int      cost;
    cpchip   cuts[0];
};

//the memory pool that manages items with certain size.
struct cerpool {
    int     chipsz;
    int     total ;
    int     cost  ;
    cpboard head  ;
};

void cpinit  (cerpool *pool, int sin);
void cpdeinit(cerpool *pool);

void *cpborrow(cerpool *pool);
void  cpreturn(cerpool *pool, void *load);

#if __cplusplus
}
#endif
