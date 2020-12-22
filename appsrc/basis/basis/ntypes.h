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

nclink bool NValueCastable(NValue value, NType type);

nclink bool     NBoolValue  (NValue value);
nclink int64_t  NInt64Value (NValue value);
nclink uint64_t NUInt64Value(NValue value);
nclink double   NDoubleValue(NValue value);
nclink void    *NPtrValue   (NValue value);
nclink int      NIntValue   (NValue value);
nclink unsigned NUIntValue  (NValue value);
nclink float    NFloatValue (NValue value);
