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

typedef void NObject;

#define NCreate(cls, destroy) NCreateObject(nsizeof(cls), destroy)

nfunc(NObject *, NCreateObject, (int size, void *destroy));

nfunc(NObject *, NRetain , (NObject *object));
nfunc(void     , NRelease, (NObject *object));

//if the high first byte of the pointer is "_NOBJECT_VALUE_FLAG",
//it means that it is actually a value but not pointer.
#define _NOBJECT_VALUE_FLAG 0x55
