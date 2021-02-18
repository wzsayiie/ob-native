#pragma once

#include "def.h"

#if __cplusplus
extern "C" {
#endif

typedef
struct cpboard cpboard;
struct cpboard {
    cpboard *prev;
    cpboard *next;
    int      cost;
    //cpchip dt[];
};

typedef struct cpchip {
    cpboard *board;
    //char  load[];
} cpchip;

//the memory pool that manages items with certain size.
typedef struct cerpool {
    int      chipsz;
    int      total ;
    int      cost  ;
    cpboard *first ;
} cerpool;

void cpinit  (cerpool *pool, int sin);
void cpdeinit(cerpool *pool);

void *cpborrow(cerpool *pool);
void  cpreturn(cerpool *pool, void *load);

#if __cplusplus
}
#endif
