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

void _NNumberInitWithBool  (NNumber *number, bool     value);
void _NNumberInitWithInt64 (NNumber *number, int64_t  value);
void _NNumberInitWithUInt64(NNumber *number, uint64_t value);
void _NNumberInitWithDouble(NNumber *number, double   value);
void _NNumberInitWithInt   (NNumber *number, int      value);
void _NNumberInitWithUInt  (NNumber *number, unsigned value);
void _NNumberInitWithFloat (NNumber *number, float    value);
void _NNumberInit          (NNumber *number);
void _NNumberDeinit        (NNumber *number);

nfunc(NNumber *, NNumberCreateWithBool  , (bool     value));
nfunc(NNumber *, NNumberCreateWithInt64 , (int64_t  value));
nfunc(NNumber *, NNumberCreateWithUInt64, (uint64_t value));
nfunc(NNumber *, NNumberCreateWithDouble, (double   value));
nfunc(NNumber *, NNumberCreateWithInt   , (int      value));
nfunc(NNumber *, NNumberCreateWithUInt  , (unsigned value));
nfunc(NNumber *, NNumberCreateWithFloat , (float    value));
nfunc(NNumber *, NNumberCreate          , (void));

nfunc(NNumber *, NNumberCopy, (NNumber *that));

nfunc(void, NNumberSetBool  , (NNumber *number, bool     value));
nfunc(void, NNumberSetInt64 , (NNumber *number, int64_t  value));
nfunc(void, NNumberSetUInt64, (NNumber *number, uint64_t value));
nfunc(void, NNumberSetDouble, (NNumber *number, double   value));
nfunc(void, NNumberSetInt   , (NNumber *number, int      value));
nfunc(void, NNumberSetUInt  , (NNumber *number, unsigned value));
nfunc(void, NNumberSetFloat , (NNumber *number, float    value));

nfunc(bool    , NNumberGetBool   , (NNumber *number));
nfunc(int64_t , NNumberGetInt64  , (NNumber *number));
nfunc(uint64_t, NNumberGetUInt64 , (NNumber *number));
nfunc(double  , NNumberGetDouble , (NNumber *number));
nfunc(int     , NNumberGetInt    , (NNumber *number));
nfunc(unsigned, NNumberGetUInt   , (NNumber *number));
nfunc(float   , NNumberGetFloat  , (NNumber *number));

nfunc(int, NNumberCompare, (NNumber *number, NNumber *that));
