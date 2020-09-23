#pragma once

#include "cstr.h"

defstruct(cpair) {
    union {
        int   intk;
        cstr *strk;
    };
    union {
        int   intv;
        float fltv;
        cref  objv;
    };
};

defstruct(cranger) {
    bool  (*next)(cranger *ranger);
    cpair (*cur )(cranger *ranger);
};

#define foreach(val, range)                          \
/**/    for (cranger *__r = range; __r->next(__r); ) \
/**/    for (bool __b = true; __b; )                 \
/**/    for (val = __r->cur(__r) ; __b; __b = false)

//store a ranger on current thread.
cranger *localranger(void *ranger, size_t size);

#define RANGER_MAX_NUM  6
#define RANGER_MAX_SIZE (sizeof(void *) * 8)
