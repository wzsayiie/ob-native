#include "nrefcounted.h"
#include "nmemory.h"

nstruct(NRefCounted) {
    int count;
    void (*clear)(void *);
    int8_t object[];
};

void *NCreate(int length, void *clear) {
    NRefCounted *memory = NAlloc(nisizeof(NRefCounted) + length);
    
    memory->count = 1;
    memory->clear = clear;
    
    return memory->object;
}

void *NRetain(void *object) {
    if (object) {
        NRefCounted *memory = (NRefCounted *)object - 1;
        nsynwith(memory) {
            memory->count += 1;
        }
    }
    return object;
}

void NRelease(void *object) {
    if (!object) {
        return;
    }
    
    NRefCounted *memory = (NRefCounted *)object - 1;
    nsynwith(memory) {
        if (--(memory->count) > 0) {
            break;
        }
        
        if (memory->clear) {
            memory->clear(object);
        }
        NFree(memory);
    }
}
