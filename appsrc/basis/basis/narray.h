#pragma once

#include "niterator.h"
#include "nmemory.h"
#include "npod.h"

nenum(__NArrayItemType) {
    __NArrayItemTypeObject = 1,
    __NArrayItemTypePOD    = 2,
};

nclass(__NArray);

nclink __NArray *__NArrayCreate(__NArrayItemType type);
nclink __NArray *__NArrayCopy  (__NArray *that);
nclink int       __NArrayCount (__NArray *self);
nclink void      __NArrayPush  (__NArray *self, NWord item);
nclink void      __NArrayPop   (__NArray *self);
nclink void      __NArrayInsert(__NArray *self, int pos, NWord item);
nclink void      __NArrayRemove(__NArray *self, int pos);
nclink void      __NArraySet   (__NArray *self, int pos, NWord item);
nclink NWord     __NArrayGet   (__NArray *self, int pos);

nclass(NArray);

nfunc(NArray  *, NArrayCreate, (void));
nfunc(NArray  *, NArrayCopy  , (NArray *that));
nfunc(int      , NArrayCount , (NArray *self));
nfunc(void     , NArrayPush  , (NArray *self, NObject *item));
nfunc(void     , NArrayPop   , (NArray *self));
nfunc(void     , NArrayInsert, (NArray *self, int pos, NObject *item));
nfunc(void     , NArrayRemove, (NArray *self, int pos));
nfunc(void     , NArraySet   , (NArray *self, int pos, NObject *item));
nfunc(NObject *, NArrayGet   , (NArray *self, int pos));

nclass(NIntArray);

nfunc(NIntArray *, NIntArrayCreate, (void));
nfunc(NIntArray *, NIntArrayCopy  , (NIntArray *that));
nfunc(int        , NIntArrayCount , (NIntArray *self));
nfunc(void       , NIntArrayPush  , (NIntArray *self, int64_t item));
nfunc(void       , NIntArrayPop   , (NIntArray *self));
nfunc(void       , NIntArrayInsert, (NIntArray *self, int pos, int64_t item));
nfunc(void       , NIntArrayRemove, (NIntArray *self, int pos));
nfunc(void       , NIntArraySet   , (NIntArray *self, int pos, int64_t item));
nfunc(int64_t    , NIntArrayGet   , (NIntArray *self, int pos));

nclass(NFltArray);

nfunc(NFltArray *, NFltArrayCreate, (void));
nfunc(NFltArray *, NFltArrayCopy  , (NFltArray *that));
nfunc(int        , NFltArrayCount , (NFltArray *self));
nfunc(void       , NFltArrayPush  , (NFltArray *self, double item));
nfunc(void       , NFltArrayPop   , (NFltArray *self));
nfunc(void       , NFltArrayInsert, (NFltArray *self, int pos, double item));
nfunc(void       , NFltArrayRemove, (NFltArray *self, int pos));
nfunc(void       , NFltArraySet   , (NFltArray *self, int pos, double item));
nfunc(double     , NFltArrayGet   , (NFltArray *self, int pos));
