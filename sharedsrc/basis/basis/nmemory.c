#include "nmemory.h"
#include "nlock.h"
#include <stdlib.h>
#include <string.h>

//pod memory management:

nstruct(MemoryBlock, {
    union {
        void *padding ;
        int   loadSize;
    };
    int8_t load[];
});

static void *Calloc (int   num, int size) {return calloc ((size_t)num, (size_t)size);}
static void *Realloc(void *ptr, int size) {return realloc(/* v* */ptr, (size_t)size);}

void *NAllocMemory(int size) {
    if (size <= 0) {
        return NULL;
    }
    
    int blockSize = nsizeof(MemoryBlock) + size;
    
    //NOTE: "calloc" will initialize all bytes to zero.
    MemoryBlock *block = Calloc(1, blockSize);
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
    
    MemoryBlock *block = (MemoryBlock *)ptr - 1;
    int oldBlockSize = nsizeof(MemoryBlock) + block->loadSize;
    int newBlockSize = nsizeof(MemoryBlock) + size;
    
    block = Realloc(block, newBlockSize);
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
    
    MemoryBlock *srcBlock = (MemoryBlock *)ptr - 1;
    int blockSize = nsizeof(MemoryBlock) + srcBlock->loadSize;

    MemoryBlock *dstBlock = Calloc(1, blockSize);
    NMoveMemory(dstBlock, srcBlock, blockSize);
    return dstBlock->load;
}

void NFreeMemory(void *ptr) {
    if (ptr) {
        MemoryBlock *block = (MemoryBlock *)ptr - 1;
        free(block);
    }
}

int NMemorySize(const void *ptr) {
    if (ptr) {
        MemoryBlock *block = (MemoryBlock *)ptr - 1;
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

nstruct(ObjectBlock, {
    void (*destroy)(NObject *);
    int    refCount;
    int8_t object[];
});

void *NCreateObject(int size, void *destroy) {
    if (size <= 0) {
        return NULL;
    }
    
    //NOTE: "calloc" will initialize all bytes to zero.
    ObjectBlock *block = Calloc(1, nsizeof(ObjectBlock) + size);
    
    block->destroy  = destroy;
    block->refCount = 1;
    
    return block->object;
}

static bool IsValue(NObject *object) {
  #if NPTR_64
    return (((uint64_t)object >> 7) & 0xff) == _NOBJECT_VAL_FLAG;
  #else
    return (((uint32_t)object >> 3) & 0xff) == _NOBJECT_VAL_FLAG;
  #endif
}

NObject *NRetain(NObject *object) {
    if (!object) {
        return NULL;
    }
    if (IsValue(object)) {
        return object;
    }
    
    for_lock(object) {
        ObjectBlock *block = (ObjectBlock *)object - 1;
        block->refCount += 1;
    }
    return object;
}

void NRelease(NObject *object) {
    if (!object) {
        return;
    }
    if (IsValue(object)) {
        return;
    }
    
    for_lock(object) {
        ObjectBlock *block = (ObjectBlock *)object - 1;
        block->refCount -= 1;
        
        if (block->refCount <= 0) {
            if (block->destroy) {
                block->destroy(object);
            }
            free(block);
        }
    }
}
