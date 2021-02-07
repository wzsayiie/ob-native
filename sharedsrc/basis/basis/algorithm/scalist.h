#pragma once

#include "def.h"

#if __cplusplus
extern "C" {
#endif

//unordered list with scalable tail.
typedef struct scalist {
    word *dat;
    int   cnt;
    int   eom;
} scalist;

void slinit  (scalist *list);
void sldeinit(scalist *list);

void slinsert(scalist *list, int  idx, word itm);
word slremove(scalist *list, int  idx);
void slpush  (scalist *list, word itm);
word slpop   (scalist *list);

int slcount(scalist *list);

void slset(scalist *list, int idx, word itm);
word slget(scalist *list, int idx);

#if __cplusplus
}
#endif
