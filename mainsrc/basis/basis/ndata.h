#pragma once

#include "nmemory.h"

nstruct(NData);

nfunc(NData *, NDataCreateWithBytes, (const void *bytes, int size));
nfunc(NData *, NDataCreateWithSize , (int size));
nfunc(NData *, NDataCreate         , (void));

nfunc(NData *, NDataCopy, (NData *that));

nfunc(void  , NDataResize, (NData *self, int size));
nfunc(void *, NDataBytes , (NData *self));
nfunc(int   , NDataSize  , (NData *self));
