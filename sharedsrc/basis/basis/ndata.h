#pragma once

#include "nmemory.h"

nclass(NData, NObject, {
    union {
        NObject Object;
        NObject Super ;
    };

    void *bytes;
});

void _NDataInitWithBytes(NData *data, const void *bytes, int size);
void _NDataInitWithSize (NData *data, int size);
void _NDataDeinit       (NData *data);

nfunc(NData *, NDataCreateWithBytes, (const void *bytes, int size));
nfunc(NData *, NDataCreateWithSize , (int size));
nfunc(NData *, NDataCreate         , (void));

nfunc(NData *, NDataCopy, (NData *that));

nfunc(void  , NDataResize, (NData *data, int size));
nfunc(void *, NDataBytes , (NData *data));
nfunc(int   , NDataSize  , (NData *data));
