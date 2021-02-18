#include "nmemory.h"
#include "nlock.h"
#include <stdlib.h>
#include <string.h>

//pod memory management:

nstruct(NMemoryBlock, {
    union {
        void *padding ;
        int   loadSize;
    };
    int8_t load[];
});

static void *SysCalloc (int   c, int s) {return calloc ((size_t)c, (size_t)s);}
static void *SysRealloc(void *p, int s) {return realloc(/* v* */p, (size_t)s);}

void *NAllocMemory(int size) {
    if (size <= 0) {
        return NULL;
    }
    
    int blockSize = nsizeof(NMemoryBlock) + size;
    
    //NOTE: "calloc" will initialize all bytes to zero.
    NMemoryBlock *block = SysCalloc(1, blockSize);
    block->loadSize = size;
    return block->load;
}

void *NReallocMemory(void *ptr, int size) {
    if (size <= 0) {
        free(ptr);
        return NULL;
    }
    if (!ptr) {
        return NAllocMemory(size);
    }
    
    NMemoryBlock *block = (NMemoryBlock *)ptr - 1;
    int oldBlockSize = nsizeof(NMemoryBlock) + block->loadSize;
    int newBlockSize = nsizeof(NMemoryBlock) + size;
    
    block = SysRealloc(block, newBlockSize);
    block->loadSize = size;
    
    //NOTE: initialize new bytes.
    if (newBlockSize > oldBlockSize) {
        void *dst = (int8_t *)block + oldBlockSize;
        int   len = newBlockSize - oldBlockSize;
        
        NZeroMemory(dst, len);
    }
    
    return block->load;
}

void *NDupMemory(const void *ptr) {
    if (!ptr) {
        return NULL;
    }
    
    NMemoryBlock *srcBlock = (NMemoryBlock *)ptr - 1;
    int blockSize = nsizeof(NMemoryBlock) + srcBlock->loadSize;

    NMemoryBlock *dstBlock = SysCalloc(1, blockSize);
    NMoveMemory(dstBlock, srcBlock, blockSize);
    return dstBlock->load;
}

void NFreeMemory(void *ptr) {
    if (ptr) {
        NMemoryBlock *block = (NMemoryBlock *)ptr - 1;
        free(block);
    }
}

int NMemorySize(const void *ptr) {
    if (ptr) {
        NMemoryBlock *block = (NMemoryBlock *)ptr - 1;
        return block->loadSize;
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

//object managed with reference counting:

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
