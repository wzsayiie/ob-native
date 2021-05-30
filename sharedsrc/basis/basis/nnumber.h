#pragma once

#include "nmemory.h"

nstruct(NNumber, {
    NType type;
});

nfunc(NNumber *, NNumberCreateWithBool  , (bool     raw));
nfunc(NNumber *, NNumberCreateWithInt   , (int32_t  raw));
nfunc(NNumber *, NNumberCreateWithInt64 , (int64_t  raw));
nfunc(NNumber *, NNumberCreateWithUInt  , (uint32_t raw));
nfunc(NNumber *, NNumberCreateWithUInt64, (uint64_t raw));
nfunc(NNumber *, NNumberCreateWithFloat , (float    raw));
nfunc(NNumber *, NNumberCreateWithDouble, (double   raw));

nfunc(NNumber *, NNumberCopy, (NNumber *that));

nfunc(bool    , NNumberGetBool   , (NNumber *number));
nfunc(int     , NNumberGetInt    , (NNumber *number));
nfunc(int64_t , NNumberGetInt64  , (NNumber *number));
nfunc(unsigned, NNumberGetUInt   , (NNumber *number));
nfunc(uint64_t, NNumberGetUInt64 , (NNumber *number));
nfunc(float   , NNumberGetFloat  , (NNumber *number));
nfunc(double  , NNumberGetDouble , (NNumber *number));

nfunc(int, NNumberCompare, (NNumber *number, NNumber *that));
