#pragma once

#include "nenviron.h"
#include "npod.h"

typedef struct NIterator NIterator;

typedef bool  (*NIteratorNext)(NIterator *iterator);
typedef NWord (*NIteratorCurr)(NIterator *iterator);

nstruct(NIterator, {
    NIteratorNext next; //move cursor to next position.
    NIteratorCurr curr; //get current value.
});

nfunc(NIterator *, NStoreIterator, (void *iterator, int size));

#define nfor(type, value, iterator)                                        \
/**/    for (NIterator *_i = iterator              ; _i && _i->next(_i); ) \
/**/    for (NWord      _v = _i->curr(_i), **_a = 0; !_a++             ; ) \
/**/    for (type    value = *(type *)&_v, **_b = 0; !_b++             ; )
