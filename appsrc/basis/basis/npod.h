#pragma once

#include "nenviron.h"

nstruct(NWord) {
    union {
        bool      asBool  ;
        int8_t    asInt8  ;
        int16_t   asInt16 ;
        int32_t   asInt32 ;
        int64_t   asInt64 ;
        int       asInt   ;
        uint8_t   asUInt8 ;
        uint16_t  asUInt16;
        uint32_t  asUInt32;
        uint64_t  asUInt64;
        unsigned  asUInt  ;
        float     asFloat ;
        double    asDouble;
        void     *asPtr   ;
    };
};

nfunc(int64_t, NMakeBoolWord  , (bool     value));
nfunc(int64_t, NMakeInt8Word  , (int8_t   value));
nfunc(int64_t, NMakeInt16Word , (int16_t  value));
nfunc(int64_t, NMakeInt32Word , (int32_t  value));
nfunc(int64_t, NMakeInt64Word , (int64_t  value));
nfunc(int64_t, NMakeIntWord   , (int      value));
nfunc(int64_t, NMakeUInt8Word , (uint8_t  value));
nfunc(int64_t, NMakeUInt16Word, (uint16_t value));
nfunc(int64_t, NMakeUInt32Word, (uint32_t value));
nfunc(int64_t, NMakeUInt64Word, (uint64_t value));
nfunc(int64_t, NMakeUIntWord  , (unsigned value));
nfunc(int64_t, NMakeFloatWord , (float    value));
nfunc(int64_t, NMakeDoubleWord, (double   value));
nfunc(int64_t, NMakePtrWord   , (void    *value));

nfunc(bool    , NBoolWord  , (int64_t value));
nfunc(int8_t  , NInt8Word  , (int64_t value));
nfunc(int16_t , NInt16Word , (int64_t value));
nfunc(int32_t , NInt32Word , (int64_t value));
nfunc(int64_t , NInt64Word , (int64_t value));
nfunc(int     , NIntWord   , (int64_t value));
nfunc(uint8_t , NUInt8Word , (int64_t value));
nfunc(uint16_t, NUInt16Word, (int64_t value));
nfunc(uint32_t, NUInt32Word, (int64_t value));
nfunc(uint64_t, NUInt64Word, (int64_t value));
nfunc(unsigned, NUIntWord  , (int64_t value));
nfunc(float   , NFloatWord , (int64_t value));
nfunc(double  , NDoubleWord, (int64_t value));
nfunc(void *  , NPtrWord   , (int64_t value));

nstruct(NValue) {
    NType type;
    union {
        bool     asBool  ;
        int64_t  asInt64 ;
        uint64_t asUInt64;
        double   asDouble;
        void    *asPtr   ;
    };
};

nclink NValue NMakeBoolValue  (bool     value);
nclink NValue NMakeInt64Value (int64_t  value);
nclink NValue NMakeUInt64Value(uint64_t value);
nclink NValue NMakeDoubleValue(double   value);
nclink NValue NMakePtrValue   (void    *value);
nclink NValue NMakeIntValue   (int      value);
nclink NValue NMakeUIntValue  (unsigned value);
nclink NValue NMakeFloatValue (float    value);

nclink bool     NBoolValue  (NValue value);
nclink int64_t  NInt64Value (NValue value);
nclink uint64_t NUInt64Value(NValue value);
nclink double   NDoubleValue(NValue value);
nclink void    *NPtrValue   (NValue value);
nclink int      NIntValue   (NValue value);
nclink unsigned NUIntValue  (NValue value);
nclink float    NFloatValue (NValue value);
