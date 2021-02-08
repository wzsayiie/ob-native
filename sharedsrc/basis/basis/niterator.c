#include "niterator.h"
#include "nmemory.h"

static bool IteratorNext(NIterator *iterator) {
    return false;
}

static void *IteratorGet(NIterator *iterator) {
    return NULL;
}

#define ITERATOR_NUM  16
#define ITERATOR_SIZE (8 * sizeof(void *))

static nthreadlocal uint8_t sStore[ITERATOR_NUM][ITERATOR_SIZE];
static nthreadlocal int32_t sIndex = 0;

NIterator *NStoreIterator(void *iterator, int size) {
    sIndex = (sIndex + 1) % ITERATOR_NUM;

    NIterator *store = (NIterator *)sStore[sIndex];
    if (iterator && 0 < size && size <= ITERATOR_SIZE) {
        NMoveMemory(store, iterator, size);
    } else {
        //empty iterator.
        store->next = IteratorNext;
        store->get  = IteratorGet ;
    }
    return store;
}
