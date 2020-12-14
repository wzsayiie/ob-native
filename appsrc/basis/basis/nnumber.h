#pragma once

#include "nmemory.h"

nstruct(NNumber);

nfunc(NNumber *, NNumberCreateWithBool, (bool    value));
nfunc(NNumber *, NNumberCreateWithInt , (int64_t value));
nfunc(NNumber *, NNumberCreateWithFlt , (double  value));
nfunc(NNumber *, NNumberCreate        , (void));

nfunc(NNumber *, NNumberCopy, (NNumber *that));

nfunc(void, NNumberSetBool, (NNumber *self, bool    value));
nfunc(void, NNumberSetInt , (NNumber *self, int64_t value));
nfunc(void, NNumberSetFlt , (NNumber *self, double  value));

nfunc(bool   , NNumberGetBool  , (NNumber *self));
nfunc(int64_t, NNumberGetInt64 , (NNumber *self));
nfunc(int    , NNumberGetInt   , (NNumber *self));
nfunc(double , NNumberGetDouble, (NNumber *self));
nfunc(float  , NNumberGetFloat , (NNumber *self));

nfunc(int, NNumberCompare, (NNumber *self, NNumber *that));
