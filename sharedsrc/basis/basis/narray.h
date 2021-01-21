#pragma once

#include "niterator.h"
#include "nmemory.h"
#include "npod.h"

nstruct(NWordArrayConf) {
    bool itemRetain;
    int  itemSize;
};

nclass(NWordArray);

nclink NWordArray *NWordArrayCreate(NWordArrayConf *conf);
nclink NWordArray *NWordArrayCopy  (NWordArray *that);
nclink int         NWordArrayCount (NWordArray *self);
nclink NIterator  *NWordArrayItems (NWordArray *self);
nclink void        NWordArrayPush  (NWordArray *self, NWord item);
nclink void        NWordArrayPop   (NWordArray *self);
nclink void        NWordArrayInsert(NWordArray *self, int index, NWord item);
nclink void        NWordArrayRemove(NWordArray *self, int index);
nclink void        NWordArraySet   (NWordArray *self, int index, NWord item);
nclink NWord       NWordArrayGet   (NWordArray *self, int index);

nclass(NArray);

nfunc(NArray    *, NArrayCreate, (void));
nfunc(NArray    *, NArrayCopy  , (NArray *that));
nfunc(int        , NArrayCount , (NArray *self));
nfunc(NIterator *, NArrayItems , (NArray *self));
nfunc(void       , NArrayPush  , (NArray *self, NObject *item));
nfunc(void       , NArrayPop   , (NArray *self));
nfunc(void       , NArrayInsert, (NArray *self, int index, NObject *item));
nfunc(void       , NArrayRemove, (NArray *self, int index));
nfunc(void       , NArraySet   , (NArray *self, int index, NObject *item));
nfunc(NObject   *, NArrayGet   , (NArray *self, int index));

nclass(NIntArray);

nfunc(NIntArray *, NIntArrayCreate, (void));
nfunc(NIntArray *, NIntArrayCopy  , (NIntArray *that));
nfunc(int        , NIntArrayCount , (NIntArray *self));
nfunc(NIterator *, NIntArrayItems , (NIntArray *self));
nfunc(void       , NIntArrayPush  , (NIntArray *self, int64_t item));
nfunc(void       , NIntArrayPop   , (NIntArray *self));
nfunc(void       , NIntArrayInsert, (NIntArray *self, int index, int64_t item));
nfunc(void       , NIntArrayRemove, (NIntArray *self, int index));
nfunc(void       , NIntArraySet   , (NIntArray *self, int index, int64_t item));
nfunc(int64_t    , NIntArrayGet   , (NIntArray *self, int index));

nclass(NFltArray);

nfunc(NFltArray *, NFltArrayCreate, (void));
nfunc(NFltArray *, NFltArrayCopy  , (NFltArray *that));
nfunc(int        , NFltArrayCount , (NFltArray *self));
nfunc(NIterator *, NFltArrayItems , (NFltArray *self));
nfunc(void       , NFltArrayPush  , (NFltArray *self, double item));
nfunc(void       , NFltArrayPop   , (NFltArray *self));
nfunc(void       , NFltArrayInsert, (NFltArray *self, int index, double item));
nfunc(void       , NFltArrayRemove, (NFltArray *self, int index));
nfunc(void       , NFltArraySet   , (NFltArray *self, int index, double item));
nfunc(double     , NFltArrayGet   , (NFltArray *self, int index));
