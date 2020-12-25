#include "niterator.h"
#include "nmemory.h"

static bool _NIteratorNext(NIterator *iterator) {
    return false;
}

static void *_NIteratorGet(NIterator *iterator) {
    return NULL;
}

#define ITERATOR_NUM  16
#define ITERATOR_SIZE (8 * sizeof(void *))

static nthreadlocal uint8_t gStore[ITERATOR_NUM][ITERATOR_SIZE];
static nthreadlocal int32_t gIndex = 0;

NIterator *NStoreIterator(void *iterator, int size) {
    gIndex = (gIndex + 1) % ITERATOR_NUM;

    NIterator *store = (NIterator *)gStore[gIndex];
    if (iterator && 0 < size && size <= ITERATOR_SIZE) {
        NMoveMemory(store, iterator, size);
    } else {
        //empty iterator.
        store->next = _NIteratorNext;
        store->get  = _NIteratorGet ;
    }
    return store;
}
