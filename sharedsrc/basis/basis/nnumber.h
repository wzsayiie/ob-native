#pragma once

#include "nmemory.h"
#include "npod.h"

nclass(NNumber, NObject, {
    union {
        NObject Object;
        NObject Super ;
    };

    NValue value;
});

void _NNumberInitWithBool  (NNumber *number, bool     raw);
void _NNumberInitWithInt   (NNumber *number, int      raw);
void _NNumberInitWithInt64 (NNumber *number, int64_t  raw);
void _NNumberInitWithUInt  (NNumber *number, unsigned raw);
void _NNumberInitWithUInt64(NNumber *number, uint64_t raw);
void _NNumberInitWithFloat (NNumber *number, float    raw);
void _NNumberInitWithDouble(NNumber *number, double   raw);
void _NNumberInit          (NNumber *number);
void _NNumberDeinit        (NNumber *number);

nfunc(NNumber *, NNumberCreateWithBool  , (bool     raw));
nfunc(NNumber *, NNumberCreateWithInt   , (int      raw));
nfunc(NNumber *, NNumberCreateWithInt64 , (int64_t  raw));
nfunc(NNumber *, NNumberCreateWithUInt  , (unsigned raw));
nfunc(NNumber *, NNumberCreateWithUInt64, (uint64_t raw));
nfunc(NNumber *, NNumberCreateWithFloat , (float    raw));
nfunc(NNumber *, NNumberCreateWithDouble, (double   raw));
nfunc(NNumber *, NNumberCreate          , (void));

nfunc(NNumber *, NNumberCopy, (NNumber *that));

nfunc(void, NNumberSetBool  , (NNumber *number, bool     raw));
nfunc(void, NNumberSetInt   , (NNumber *number, int      raw));
nfunc(void, NNumberSetInt64 , (NNumber *number, int64_t  raw));
nfunc(void, NNumberSetUInt  , (NNumber *number, unsigned raw));
nfunc(void, NNumberSetUInt64, (NNumber *number, uint64_t raw));
nfunc(void, NNumberSetFloat , (NNumber *number, float    raw));
nfunc(void, NNumberSetDouble, (NNumber *number, double   raw));

nfunc(bool    , NNumberGetBool   , (NNumber *number));
nfunc(int     , NNumberGetInt    , (NNumber *number));
nfunc(int64_t , NNumberGetInt64  , (NNumber *number));
nfunc(unsigned, NNumberGetUInt   , (NNumber *number));
nfunc(uint64_t, NNumberGetUInt64 , (NNumber *number));
nfunc(float   , NNumberGetFloat  , (NNumber *number));
nfunc(double  , NNumberGetDouble , (NNumber *number));

nfunc(int, NNumberCompare, (NNumber *number, NNumber *that));
