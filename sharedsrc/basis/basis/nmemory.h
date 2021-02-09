#pragma once

#include "nenviron.h"

nfunc(void *, NAllocMemory  , (int size));
nfunc(void *, NReallocMemory, (void *ptr, int size));
nfunc(void *, NDupMemory    , (const void *ptr));
nfunc(void  , NFreeMemory   , (void *ptr));

nfunc(int , NMemorySize, (const void *ptr));
nfunc(void, NZeroMemory, (void *ptr, int size));
nfunc(void, NMoveMemory, (void *dst, const void *src, int size));

typedef void NObject;

//NCreate : allocate a pod object and set its reference count to 1.
//NRetain : increment the reference count by 1.
//NRelease: decrease the count by 1. if the count reduced to 0, delete the object.
nfunc(NObject *, NCreate , (int      length, void *clear));
nfunc(NObject *, NRetain , (NObject *object));
nfunc(void     , NRelease, (NObject *object));
