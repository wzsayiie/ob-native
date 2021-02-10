#include "nmemory.h"
#include "nlock.h"
#include <stdlib.h>
#include <string.h>

nstruct(NMemory, {
    int    dataSize;
    int8_t data[];
});

static size_t MemorySize(int size) {
    return sizeof(NMemory) + (size_t)size;
}

static NMemory *MemoryPtr(const void *ptr) {
    return (NMemory *)ptr - 1;
}

void *NAllocMemory(int size) {
    if (size <= 0) {
        return NULL;
    }
    
    size_t   mmSize = MemorySize(size);
    NMemory *memory = calloc(1, mmSize);
    if (memory) {
        memory->dataSize = size;
        return memory->data;
    } else {
        //allocation failed.
        return NULL;
    }
}

void *NReallocMemory(void *ptr, int size) {
    if (size <= 0) {
        free(ptr);
        return NULL;
    }
    if (!ptr) {
        return NAllocMemory(size);
    }
    
    NMemory *oldMem = MemoryPtr(ptr);

    size_t   mmSize = MemorySize(size);
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

void *NDupMemory(const void *ptr) {
    if (!ptr) {
        return NULL;
    }

    NMemory *memory = MemoryPtr(ptr);
    size_t   mmSize = MemorySize(memory->dataSize);

    NMemory *dup = calloc(1, mmSize);
    if (dup) {
        memcpy(dup, memory, mmSize);
        return dup->data;
    } else {
        return NULL;
    }
}

void NFreeMemory(void *ptr) {
    if (ptr) {
        NMemory *memory = MemoryPtr(ptr);
        free(memory);
    }
}

int NMemorySize(const void *ptr) {
    if (ptr) {
        NMemory *memory = MemoryPtr(ptr);
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

void *NAllocObj(int size, void *deinit) {
    NObject *object = NAllocMemory(size);
    
    object->deinit = deinit;
    object->refCount = 1;
    
    return object;
}

NRef NRetain(NRef ref) {
    if (ref) {
        nsynwith(ref) {
            NObject *object = ref;
            object->refCount += 1;
        }
    }
    return ref;
}

void NRelease(NRef ref) {
    if (!ref) {
        return;
    }
    
    nsynwith(ref) {
        NObject *object = ref;
        if (--(object->refCount) > 0) {
            break;
        }
        
        if (object->deinit) {
            object->deinit(object);
        }
        NFreeMemory(object);
    }
}

void _NObjectInit  (NObject *object) {}
void _NObjectDeinit(NObject *object) {}

NObject *NObjectCreate(void) {
    NObject *object = NAlloc(NObject, _NObjectDeinit);
    _NObjectInit(object);
    return object;
}
