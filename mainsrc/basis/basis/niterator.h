#pragma once

#include "ndef.h"

nstruct(NIterator) {
    bool  (*Next)(NIterator *iterator);
    void *(*Get )(NIterator *iterator);
};

nfunc(NIterator *, NIteratorGetEmpty)(void);

#define nfor(type, value, iterator) \
/**/    for (NIterator *__i = iterator                            ; __i           ; __i = 0) \
/**/    for (                                                     ; __i->Next(__i);        ) \
/**/    for (type value = *(type *)__i->Get(__i), *__b = (type *)1; __b           ; __b = 0)
