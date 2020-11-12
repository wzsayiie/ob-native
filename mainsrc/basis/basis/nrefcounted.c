#include "nrefcounted.h"
#include "ndef.h"
#include "nmemory.h"

nstruct(NRefCounted) {
    int    count;
    void (*clear)(void *);
    char   block[];
};

void *NCreate(int bsize, void *clear) {
    NRefCounted *ref = NAlloc(nisizeof(NRefCounted) + bsize);
    ref->count = 1;
    ref->clear = clear;
    return ref->block;
}

void *NRetain(void *block) {
    if (block) {
        NRefCounted *ref = (NRefCounted *)block - 1;
        nsynwith(ref) {
            ref->count += 1;
        }
    }
    return block;
}

void NRelease(void *block) {
    if (!block) {
        return;
    }
    
    NRefCounted *ref = (NRefCounted *)block - 1;
    nsynwith(ref) {
        if (--(ref->count) <= 0) {
            if (ref->clear) {
                ref->clear(ref->block);
            }
            NFree(ref);
        }
    }
}
