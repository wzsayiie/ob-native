#pragma once

#include "ndef.h"

nfunc(void *, NAlloc  , (int size));
nfunc(void *, NRealloc, (void *ptr, int size));
nfunc(void *, NDup    , (const void *ptr));
nfunc(void  , NFree   , (void *ptr));

nfunc(int , NMemorySize, (const void *ptr));
nfunc(void, NZeroMemory, (void *ptr, int size));
nfunc(void, NMoveMemory, (void *dst, const void *src, int size));
