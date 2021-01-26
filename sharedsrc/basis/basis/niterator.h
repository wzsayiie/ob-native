#pragma once

#include "nenviron.h"

typedef struct NIterator NIterator;

typedef bool  (*NIteratorNextFunc)(NIterator *iterator);
typedef void *(*NIteratorGetFunc )(NIterator *iterator);

nstruct(NIterator, {
    NIteratorNextFunc next;
    NIteratorGetFunc  get ;
});

nfunc(NIterator *, NStoreIterator, (void *iterator, int size));

#define nfor(type, value, iterator) \
/**/    for (NIterator *__i = iterator                        ; __i           ; __i = 0) \
/**/    for (                                                 ; __i->next(__i);        ) \
/**/    for (type value = *(type *)__i->get(__i), **__b = NULL; !__b          ; ++__b  )
