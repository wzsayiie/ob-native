#pragma once

#include "nmemory.h"

nstruct(NNumber);

nfunc(NNumber *, NNumberCreateWithBool, (bool     value));
nfunc(NNumber *, NNumberCreateWithInt , (int64_t  value));
nfunc(NNumber *, NNumberCreateWithUInt, (uint64_t value));
nfunc(NNumber *, NNumberCreateWithDbl , (double   value));
nfunc(NNumber *, NNumberCreate        , (void));

nfunc(NNumber *, NNumberCopy, (NNumber *that));

nfunc(void, NNumberSetBool, (NNumber *self, bool     value));
nfunc(void, NNumberSetInt , (NNumber *self, int64_t  value));
nfunc(void, NNumberSetUInt, (NNumber *self, uint64_t value));
nfunc(void, NNumberSetDbl , (NNumber *self, double   value));

nfunc(bool    , NNumberGetBool   , (NNumber *self));
nfunc(int64_t , NNumberGetInt64  , (NNumber *self));
nfunc(int     , NNumberGetInt    , (NNumber *self));
nfunc(uint64_t, NNumberGetUInt64 , (NNumber *self));
nfunc(unsigned, NNumberGetUInt   , (NNumber *self));
nfunc(double  , NNumberGetDouble , (NNumber *self));
nfunc(float   , NNumberGetFloat  , (NNumber *self));

nfunc(int, NNumberCompare, (NNumber *self, NNumber *that));
