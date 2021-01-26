#pragma once

#include "nmemory.h"

nclass(NNumber, NObject);

nfunc(NNumber *, NNumberCreateWithBool  , (bool     value));
nfunc(NNumber *, NNumberCreateWithInt64 , (int64_t  value));
nfunc(NNumber *, NNumberCreateWithUInt64, (uint64_t value));
nfunc(NNumber *, NNumberCreateWithDouble, (double   value));
nfunc(NNumber *, NNumberCreateWithInt   , (int      value));
nfunc(NNumber *, NNumberCreateWithUInt  , (unsigned value));
nfunc(NNumber *, NNumberCreateWithFloat , (float    value));
nfunc(NNumber *, NNumberCreate          , (void));

nfunc(NNumber *, NNumberCopy, (NNumber *that));

nfunc(void, NNumberSetBool  , (NNumber *self, bool     value));
nfunc(void, NNumberSetInt64 , (NNumber *self, int64_t  value));
nfunc(void, NNumberSetUInt64, (NNumber *self, uint64_t value));
nfunc(void, NNumberSetDouble, (NNumber *self, double   value));
nfunc(void, NNumberSetInt   , (NNumber *self, int      value));
nfunc(void, NNumberSetUInt  , (NNumber *self, unsigned value));
nfunc(void, NNumberSetFloat , (NNumber *self, float    value));

nfunc(bool    , NNumberGetBool   , (NNumber *self));
nfunc(int64_t , NNumberGetInt64  , (NNumber *self));
nfunc(uint64_t, NNumberGetUInt64 , (NNumber *self));
nfunc(double  , NNumberGetDouble , (NNumber *self));
nfunc(int     , NNumberGetInt    , (NNumber *self));
nfunc(unsigned, NNumberGetUInt   , (NNumber *self));
nfunc(float   , NNumberGetFloat  , (NNumber *self));

nfunc(int, NNumberCompare, (NNumber *self, NNumber *that));
