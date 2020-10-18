#include "nmemory.h"
#include "ndef.h"
#include <stdlib.h>
#include <string.h>

nstruct(NMemoryBlock) {
    int  dataSize;
    char data[];
};

void *NAlloc(int size) {
    if (size > 0) {
        //"calloc" will set all bytes of the memory block to 0.
        NMemoryBlock *block = calloc(1, sizeof(NMemoryBlock) + (size_t)size);
        block->dataSize = size;
        return block->data;
    }
    return NULL;
}

void *NRealloc(void *ptr, int size) {
    if (ptr) {
        NMemoryBlock *block = (NMemoryBlock *)ptr - 1;
        if (size > 0) {
            block = realloc(block, sizeof(NMemoryBlock) + (size_t)size);
            block->dataSize = size;
            return block->data;
        } else {
            free(block);
            return NULL;
        }
    }
    return NULL;
}

void NFree(void *ptr) {
    if (ptr) {
        NMemoryBlock *block = (NMemoryBlock *)ptr - 1;
        free(block);
    }
}

int NMemorySize(void *ptr) {
    if (ptr) {
        NMemoryBlock *block = (NMemoryBlock *)ptr - 1;
        return block->dataSize;
    }
    return 0;
}

void NZeroMemory(void *ptr, int size) {
    memset(ptr, 0, (size_t)size);
}

void NMoveMemory(void *dst, const void *src, int size) {
    memmove(dst, src, (size_t)size);
}
