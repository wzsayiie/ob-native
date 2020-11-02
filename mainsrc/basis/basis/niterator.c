#include "niterator.h"

static bool _NIteratorNext(NIterator *iterator) {
    return false;
}

static void *_NIteratorGet(NIterator *iterator) {
    return NULL;
}

NIterator *NIteratorGetEmpty(void) {
    static NIterator iterator = {
        _NIteratorNext,
        _NIteratorGet,
    };
    return &iterator;
}
