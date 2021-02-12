#pragma once

#include "def.h"

#if __cplusplus
extern "C" {
#endif

typedef struct blitem {
    word key;
    word val;
} blitem;

//ordered list that managed using binary insertion.
typedef struct binlist {
    cmpr    cmp;
    blitem *dat;
    int     cnt;
    int     eom;
} binlist;

void blinit  (binlist *list, cmpr cmp);
void bldeinit(binlist *list);

//NOTE: it's allowed to store multiple values with the same key.
int  blinsert(binlist *list, word key, word val);
word blremove(binlist *list, word key);
word blcover (binlist *list, int  idx);

int bladdup(binlist *list, word key);
int blcount(binlist *list);

word blvalue (binlist *list, word key);
int  blindex (binlist *list, word key);
word bloffset(binlist *list, int  idx);

#if __cplusplus
}
#endif
