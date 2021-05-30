#pragma once

#include "nmemory.h"
#include "scalist.h"

nstruct(NArray, {
    scalist list;
});

nfunc(NArray  *, NArrayCreate, (void));
nfunc(NArray  *, NArrayCopy  , (NArray *that ));
nfunc(int      , NArrayCount , (NArray *array));
nfunc(void     , NArrayPush  , (NArray *array, NObject *item));
nfunc(void     , NArrayPop   , (NArray *array));
nfunc(void     , NArrayInsert, (NArray *array, int index, NObject *item));
nfunc(void     , NArrayRemove, (NArray *array, int index));
nfunc(void     , NArraySet   , (NArray *array, int index, NObject *item));
nfunc(NObject *, NArrayGet   , (NArray *array, int index));
