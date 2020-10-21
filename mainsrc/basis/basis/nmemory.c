#include "nmemory.h"
#include "ndef.h"
#include <stdlib.h>
#include <string.h>

nstruct(NMemoryBlock) {
    int  dataSize;
    char data[];
};

static size_t TotalSizeWhenDataSizes(int size) {
    return sizeof(NMemoryBlock) + (size_t)size;
}

static NMemoryBlock *BlockOfDataPtr(void *ptr) {
    return (NMemoryBlock *)ptr - 1;
}

void *NAlloc(int size) {
    if (size <= 0) {
        return NULL;
    }
    
    size_t totalSize = TotalSizeWhenDataSizes(size);
    NMemoryBlock *block = calloc(1, totalSize);
    if (block) {
        block->dataSize = size;
        return block->data;
    } else {
        return NULL;
    }
}

void *NRealloc(void *ptr, int size) {
    if (!ptr) {
        return NULL;
    }
    
    NMemoryBlock *oldBlock = BlockOfDataPtr(ptr);
    if (size > 0) {
        size_t totalSize = TotalSizeWhenDataSizes(size);
        NMemoryBlock *newBlock = realloc(oldBlock, totalSize);
        if (newBlock) {
            newBlock->dataSize = size;
            return newBlock->data;
        }
    }
    
    //here "ptr", "size" are illegal, or "realloc" failed.
    free(oldBlock);
    return NULL;
}

void *NDup(void *ptr) {
    if (!ptr) {
        return NULL;
    }

    NMemoryBlock *block = BlockOfDataPtr(ptr);
    size_t totalSize = TotalSizeWhenDataSizes(block->dataSize);
    void *dup = calloc(1, totalSize);
    if (dup) {
        memcpy(dup, block, totalSize);
    }
    return dup;
}

void NFree(void *ptr) {
    if (ptr) {
        NMemoryBlock *block = BlockOfDataPtr(ptr);
        free(block);
    }
}

int NMemorySize(void *ptr) {
    if (ptr) {
        NMemoryBlock *block = BlockOfDataPtr(ptr);
        return block->dataSize;
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
