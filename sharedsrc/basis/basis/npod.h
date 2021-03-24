#pragma once

#include "nenviron.h"

typedef struct NString NString;
typedef struct NLambda NLambda;

nstruct(NWord, {
    union {
        bool      asBool    ;
        int       asInt     ;
        int64_t   asInt64   ;
        unsigned  asUInt    ;
        uint64_t  asUInt64  ;
        float     asFloat   ;
        double    asDouble  ;

        void     *asPtr     ;
        char     *asU8Chars ;
        char16_t *asU16Chars;
        char32_t *asU32Chars;

        void     *asObject  ;
        NString  *asString  ;
        NLambda  *asLambda  ;
    };
});

nstruct(NValue, {
    NType type;
    union {
        int64_t  asInt64 ;
        uint64_t asUInt64;
        double   asDouble;
        void    *asPtr   ;
        void    *asObject;
    };
});

nclink NValue NMakeVoidValue(void);

nclink NValue NMakeBoolValue  (bool     raw);
nclink NValue NMakeIntValue   (int      raw);
nclink NValue NMakeInt64Value (int64_t  raw);
nclink NValue NMakeUIntValue  (unsigned raw);
nclink NValue NMakeUInt64Value(uint64_t raw);
nclink NValue NMakeFloatValue (float    raw);
nclink NValue NMakeDoubleValue(double   raw);
nclink NValue NMakePtrValue   (void    *raw);

nclink NValue NMakeU8CharsValue (const char     *raw);
nclink NValue NMakeU16CharsValue(const char16_t *raw);
nclink NValue NMakeU32CharsValue(const char32_t *raw);

nclink NValue NMakeObjectValue(void    *raw);
nclink NValue NMakeStringValue(NString *raw);
nclink NValue NMakeLambdaValue(NLambda *raw);

nclink bool      NBoolValue    (NValue value);
nclink int       NIntValue     (NValue value);
nclink int64_t   NInt64Value   (NValue value);
nclink unsigned  NUIntValue    (NValue value);
nclink uint64_t  NUInt64Value  (NValue value);
nclink float     NFloatValue   (NValue value);
nclink double    NDoubleValue  (NValue value);
nclink void     *NPtrValue     (NValue value);
nclink char     *NU8CharsValue (NValue value);
nclink char16_t *NU16CharsValue(NValue value);
nclink char32_t *NU32CharsValue(NValue value);
nclink void     *NObjectValue  (NValue value);
nclink NString  *NStringValue  (NValue value);
nclink NLambda  *NLambdaValue  (NValue value);
