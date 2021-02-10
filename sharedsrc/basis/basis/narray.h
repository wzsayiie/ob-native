#pragma once

#include "niterator.h"
#include "nmemory.h"
#include "npod.h"

nstruct(NWordArrayConf, {
    bool itemRetain;
    int  itemSize;
});

nclass(NWordArray, NObject, {
    union {
        NObject Object;
        NObject Super ;
    };

    NWordArrayConf conf;
    void *items;
    int count;
});

void _NWordArrayInit  (NWordArray *array, NWordArrayConf *conf);
void _NWordArrayDeinit(NWordArray *array);

nclink NWordArray *NWordArrayCreate(NWordArrayConf *conf);
nclink NWordArray *NWordArrayCopy  (NWordArray *that );
nclink int         NWordArrayCount (NWordArray *array);
nclink NIterator  *NWordArrayItems (NWordArray *array);
nclink void        NWordArrayPush  (NWordArray *array, NWord item);
nclink void        NWordArrayPop   (NWordArray *array);
nclink void        NWordArrayInsert(NWordArray *array, int index, NWord item);
nclink void        NWordArrayRemove(NWordArray *array, int index);
nclink void        NWordArraySet   (NWordArray *array, int index, NWord item);
nclink NWord       NWordArrayGet   (NWordArray *array, int index);

nclass(NArray, NWordArray, {
    union {
        NObject    Object;
        NWordArray Array ;
        NWordArray Super ;
    };
});

nfunc(NArray    *, NArrayCreate, (void));
nfunc(NArray    *, NArrayCopy  , (NArray *that ));
nfunc(int        , NArrayCount , (NArray *array));
nfunc(NIterator *, NArrayItems , (NArray *array));
nfunc(void       , NArrayPush  , (NArray *array, NRef item));
nfunc(void       , NArrayPop   , (NArray *array));
nfunc(void       , NArrayInsert, (NArray *array, int index, NRef item));
nfunc(void       , NArrayRemove, (NArray *array, int index));
nfunc(void       , NArraySet   , (NArray *array, int index, NRef item));
nfunc(NRef       , NArrayGet   , (NArray *array, int index));

nclass(NIntArray, NWordArray, {
    union {
        NObject    Object;
        NWordArray Super ;
    };
});

nfunc(NIntArray *, NIntArrayCreate, (void));
nfunc(NIntArray *, NIntArrayCopy  , (NIntArray *that ));
nfunc(int        , NIntArrayCount , (NIntArray *array));
nfunc(NIterator *, NIntArrayItems , (NIntArray *array));
nfunc(void       , NIntArrayPush  , (NIntArray *array, int64_t item));
nfunc(void       , NIntArrayPop   , (NIntArray *array));
nfunc(void       , NIntArrayInsert, (NIntArray *array, int index, int64_t item));
nfunc(void       , NIntArrayRemove, (NIntArray *array, int index));
nfunc(void       , NIntArraySet   , (NIntArray *array, int index, int64_t item));
nfunc(int64_t    , NIntArrayGet   , (NIntArray *array, int index));

nclass(NFltArray, NWordArray, {
    union {
        NObject    Object;
        NWordArray Super ;
    };
});

nfunc(NFltArray *, NFltArrayCreate, (void));
nfunc(NFltArray *, NFltArrayCopy  , (NFltArray *that ));
nfunc(int        , NFltArrayCount , (NFltArray *array));
nfunc(NIterator *, NFltArrayItems , (NFltArray *array));
nfunc(void       , NFltArrayPush  , (NFltArray *array, double item));
nfunc(void       , NFltArrayPop   , (NFltArray *array));
nfunc(void       , NFltArrayInsert, (NFltArray *array, int index, double item));
nfunc(void       , NFltArrayRemove, (NFltArray *array, int index));
nfunc(void       , NFltArraySet   , (NFltArray *array, int index, double item));
nfunc(double     , NFltArrayGet   , (NFltArray *array, int index));
