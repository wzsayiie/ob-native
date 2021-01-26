#include "nmemory.h"
#include "nlock.h"
#include <stdlib.h>
#include <string.h>

nstruct(NMemory, {
    int    dataSize;
    int8_t data[];
});

static size_t _NMemorySize(int size) {
    return sizeof(NMemory) + (size_t)size;
}

static NMemory *_NMemoryPtr(const void *ptr) {
    return (NMemory *)ptr - 1;
}

void *NAlloc(int size) {
    if (size <= 0) {
        return NULL;
    }
    
    size_t   mmSize = _NMemorySize(size);
    NMemory *memory = calloc(1, mmSize);
    if (memory) {
        memory->dataSize = size;
        return memory->data;
    } else {
        //allocation failed.
        return NULL;
    }
}

void *NRealloc(void *ptr, int size) {
    if (size <= 0) {
        free(ptr);
        return NULL;
    }
    if (!ptr) {
        return NAlloc(size);
    }
    
    NMemory *oldMem = _NMemoryPtr(ptr);

    size_t   mmSize = _NMemorySize(size);
    NMemory *newMem = realloc(oldMem, mmSize);
    if (newMem) {
        newMem->dataSize = size;
        return newMem->data;
    } else {
        //reallocation failed.
        free(oldMem);
        return NULL;
    }
}

void *NDup(const void *ptr) {
    if (!ptr) {
        return NULL;
    }

    NMemory *memory = _NMemoryPtr(ptr);
    size_t   mmSize = _NMemorySize(memory->dataSize);

    NMemory *dup = calloc(1, mmSize);
    if (dup) {
        memcpy(dup, memory, mmSize);
        return dup->data;
    } else {
        return NULL;
    }
}

void NFree(void *ptr) {
    if (ptr) {
        NMemory *memory = _NMemoryPtr(ptr);
        free(memory);
    }
}

int NMemorySize(const void *ptr) {
    if (ptr) {
        NMemory *memory = _NMemoryPtr(ptr);
        return memory->dataSize;
    }
    return 0;
}

void NZeroMemory(void *ptr, int size) {
    if (ptr && size > 0) {
        memset(ptr, 0, (size_t)size);
    }
}

void NMoveMemory(void *dst, const void *src, int size) {
    if (dst && src && size > 0) {
        memmove(dst, src, (size_t)size);
    }
}

nstruct(NObjectCarrier, {
    int count;
    void (*clear)(void *);
    int8_t object[];
});

NObject *NCreate(int length, void *clear) {
    NObjectCarrier *carrier = NAlloc(nisizeof(NObjectCarrier) + length);
    
    carrier->count = 1;
    carrier->clear = clear;
    
    return carrier->object;
}

NObject *NRetain(NObject *object) {
    if (object) {
        NObjectCarrier *carrier = (NObjectCarrier *)object - 1;
        nsynwith(carrier) {
            carrier->count += 1;
        }
    }
    return object;
}

void NRelease(NObject *object) {
    if (!object) {
        return;
    }
    
    NObjectCarrier *carrier = (NObjectCarrier *)object - 1;
    nsynwith(carrier) {
        if (--(carrier->count) > 0) {
            break;
        }
        
        if (carrier->clear) {
            carrier->clear(object);
        }
        NFree(carrier);
    }
}
