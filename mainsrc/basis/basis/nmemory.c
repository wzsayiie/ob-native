#include "nmemory.h"
#include "nlock.h"
#include <stdlib.h>
#include <string.h>

nstruct(NMemory) {
    int    dataSize;
    int8_t data[];
};

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

    void *dup = calloc(1, mmSize);
    if (dup) {
        memcpy(dup, memory, mmSize);
    }
    return dup;
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

nstruct(NCountedMemory) {
    int count;
    void (*clear)(void *);
    int8_t object[];
};

void *NCreate(int length, void *clear) {
    NCountedMemory *memory = NAlloc(nisizeof(NCountedMemory) + length);
    
    memory->count = 1;
    memory->clear = clear;
    
    return memory->object;
}

void *NRetain(void *object) {
    if (object) {
        NCountedMemory *memory = (NCountedMemory *)object - 1;
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
    
    NCountedMemory *memory = (NCountedMemory *)object - 1;
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
