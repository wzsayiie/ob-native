#pragma once

#include "nmemory.h"

nstruct(NData, {
    void *bytes;
});

nfunc(NData *, NDataCreateWithBytes, (const void *bytes, int size));
nfunc(NData *, NDataCreateWithSize , (int size));
nfunc(NData *, NDataCreate         , (void));

nfunc(NData *, NDataCopy, (NData *that));

nfunc(void *, NDataResize, (NData *data, int size));
nfunc(void *, NDataBytes , (NData *data));
nfunc(int   , NDataSize  , (NData *data));
