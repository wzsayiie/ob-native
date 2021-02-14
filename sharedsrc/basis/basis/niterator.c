#include "niterator.h"
#include "nmemory.h"

static bool IteratorNext(NIterator *iterator) {
    return false;
}

static NWord IteratorCurr(NIterator *iterator) {
    NWord value = {0};
    return value;
}

#define MAX_NUM  16
#define MAX_SIZE (8 * sizeof(void *))

static nthreadlocal uint8_t sStore[MAX_NUM][MAX_SIZE];
static nthreadlocal int32_t sIndex = 0;

NIterator *NStoreIterator(void *iterator, int size) {
    sIndex = (sIndex + 1) % MAX_NUM;

    NIterator *store = (NIterator *)sStore[sIndex];
    if (iterator && (0 < size && size <= MAX_SIZE)) {
        NMoveMemory(store, iterator, size);
    } else {
        //empty iterator.
        store->next = IteratorNext;
        store->curr = IteratorCurr;
    }
    return store;
}
