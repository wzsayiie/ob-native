#include "nobject.h"
#include "nmemory.h"

nstruct(NObject) {
    int  reference;
    void (*deinit)(void *);
    char sub[];
};

void *NCreate(int size, void *deinit) {
    NObject *super = NAlloc(nisizeof(NObject) + size);
    super->reference = 1;
    super->deinit = deinit;
    return super->sub;
}

void *NRetain(void *object) {
    if (!object) {
        return NULL;
    }
    
    NObject *super = (NObject *)object - 1;
    nsynwith(super) {
        super->reference += 1;
    }
    return object;
}

void NRelease(void *object) {
    if (!object) {
        return;
    }
    
    NObject *super = (NObject *)object - 1;
    nsynwith(super) {
        if (--(super->reference) == 0) {
            super->deinit(super->sub);
        }
    }
}
