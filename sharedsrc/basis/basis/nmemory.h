#pragma once

#include "nenviron.h"

//pod memory management:

nfunc(void *, NAllocMemory  , (int size));
nfunc(void *, NReallocMemory, (void *ptr, int size));
nfunc(void *, NDupMemory    , (const void *ptr));
nfunc(void  , NFreeMemory   , (void *ptr));

nfunc(int , NMemorySize, (const void *ptr));
nfunc(void, NZeroMemory, (void *ptr, int size));
nfunc(void, NMoveMemory, (void *dst, const void *src, int size));

//object managed with reference counting:

typedef void *NRef;

#define NAlloc(cls, deinit) NAllocObj(nsizeof(cls), deinit)

nfunc(NRef, NAllocObj, (int size, void *deinit));
nfunc(NRef, NRetain  , (NRef ref));
nfunc(void, NRelease , (NRef ref));

#define nsuperof(object) (&(object)->Super)

nstruct(NObject, {
    void (*deinit)(void *);
    int refCount;
});

void _NObjectInit  (NObject *object);
void _NObjectDeinit(NObject *object);

nfunc(NObject *, NObjectCreate, (void));
