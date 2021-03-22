#include "npod.h"

#define RETURN_VALUE(tid, mem, raw) NValue val = {0}; val.type = tid; val.mem = raw; return val

NValue NMakeBoolValue    (bool      raw) {RETURN_VALUE(NTYPE_INT64     , asInt64 , raw);}
NValue NMakeIntValue     (int       raw) {RETURN_VALUE(NTYPE_INT64     , asInt64 , raw);}
NValue NMakeInt64Value   (int64_t   raw) {RETURN_VALUE(NTYPE_INT64     , asInt64 , raw);}
NValue NMakeUIntValue    (unsigned  raw) {RETURN_VALUE(NTYPE_UINT64    , asUInt64, raw);}
NValue NMakeUInt64Value  (uint64_t  raw) {RETURN_VALUE(NTYPE_UINT64    , asUInt64, raw);}
NValue NMakeFloatValue   (float     raw) {RETURN_VALUE(NTYPE_DOUBLE    , asDouble, raw);}
NValue NMakeDoubleValue  (double    raw) {RETURN_VALUE(NTYPE_DOUBLE    , asDouble, raw);}
NValue NMakeU8CharsValue (char     *raw) {RETURN_VALUE(NTYPE_CHAR8_PTR , asPtr   , raw);}
NValue NMakeU16CharsValue(char16_t *raw) {RETURN_VALUE(NTYPE_CHAR16_PTR, asPtr   , raw);}
NValue NMakeU32CharsValue(char32_t *raw) {RETURN_VALUE(NTYPE_CHAR32_PTR, asPtr   , raw);}
NValue NMakePtrValue     (void     *raw) {RETURN_VALUE(NTYPE_VOID_PTR  , asPtr   , raw);}
NValue NMakeObjectValue  (NObject  *raw) {RETURN_VALUE(NTYPE_OBJECT_REF, asPtr   , raw);}
NValue NMakeStringValue  (NString  *raw) {RETURN_VALUE(NTYPE_STRING_REF, asPtr   , raw);}
NValue NMakeLambdaValue  (NLambda  *raw) {RETURN_VALUE(NTYPE_LAMBDA_REF, asPtr   , raw);}

bool NBoolValue(NValue value) {
    switch (value.type) {
        case NTYPE_INT64 : return value.asInt64  != 0;
        case NTYPE_UINT64: return value.asUInt64 != 0;
        case NTYPE_DOUBLE: return value.asDouble != 0;

        case NTYPE_CHAR8_PTR : return value.asPtr != 0;
        case NTYPE_CHAR16_PTR: return value.asPtr != 0;
        case NTYPE_CHAR32_PTR: return value.asPtr != 0;
        case NTYPE_VOID_PTR  : return value.asPtr != 0;
        case NTYPE_OBJECT_REF: return value.asPtr != 0;
        case NTYPE_STRING_REF: return value.asPtr != 0;
        case NTYPE_LAMBDA_REF: return value.asPtr != 0;

        default: return false;
    }
}

int NIntValue(NValue value) {
    return (int)NInt64Value(value);
}

int64_t NInt64Value(NValue value) {
    switch (value.type) {
        case NTYPE_INT64 : return (int64_t)value.asInt64;
        case NTYPE_UINT64: return (int64_t)value.asUInt64;
        case NTYPE_DOUBLE: return (int64_t)value.asDouble;

        //it is legal to cast a pointer to an integer.
        case NTYPE_CHAR8_PTR : return (int64_t)value.asPtr;
        case NTYPE_CHAR16_PTR: return (int64_t)value.asPtr;
        case NTYPE_CHAR32_PTR: return (int64_t)value.asPtr;
        case NTYPE_VOID_PTR  : return (int64_t)value.asPtr;
        case NTYPE_OBJECT_REF: return (int64_t)value.asPtr;
        case NTYPE_STRING_REF: return (int64_t)value.asPtr;
        case NTYPE_LAMBDA_REF: return (int64_t)value.asPtr;

        default: return 0;
    }
}

unsigned NUIntValue(NValue value) {
    return (unsigned)NUInt64Value(value);
}

uint64_t NUInt64Value(NValue value) {
    switch (value.type) {
        case NTYPE_INT64 : return (uint64_t)value.asInt64 ;
        case NTYPE_UINT64: return (uint64_t)value.asUInt64;
        case NTYPE_DOUBLE: return (uint64_t)value.asDouble;

        //it is legal to cast a pointer to an unsigned integer.
        case NTYPE_CHAR8_PTR : return (uint64_t)value.asPtr;
        case NTYPE_CHAR16_PTR: return (uint64_t)value.asPtr;
        case NTYPE_CHAR32_PTR: return (uint64_t)value.asPtr;
        case NTYPE_VOID_PTR  : return (uint64_t)value.asPtr;
        case NTYPE_OBJECT_REF: return (uint64_t)value.asPtr;
        case NTYPE_STRING_REF: return (uint64_t)value.asPtr;
        case NTYPE_LAMBDA_REF: return (uint64_t)value.asPtr;

        default: return 0;
    }
}

float NFloatValue(NValue value) {
    return (float)NDoubleValue(value);
}

double NDoubleValue(NValue value) {
    switch (value.type) {
        case NTYPE_INT64 : return (double)value.asInt64 ;
        case NTYPE_UINT64: return (double)value.asUInt64;
        case NTYPE_DOUBLE: return (double)value.asDouble;

        case NTYPE_CHAR8_PTR : return 0;
        case NTYPE_CHAR16_PTR: return 0;
        case NTYPE_CHAR32_PTR: return 0;
        case NTYPE_VOID_PTR  : return 0;
        case NTYPE_OBJECT_REF: return 0;
        case NTYPE_STRING_REF: return 0;
        case NTYPE_LAMBDA_REF: return 0;

        default: return 0;
    }
}

#define RETURN_PTR(val, tid) return val.type == tid ? val.asPtr : NULL

char     *NU8CharsValue (NValue value) {RETURN_PTR(value, NTYPE_CHAR8_PTR );}
char16_t *NU16CharsValue(NValue value) {RETURN_PTR(value, NTYPE_CHAR16_PTR);}
char32_t *NU32CharsValue(NValue value) {RETURN_PTR(value, NTYPE_CHAR32_PTR);}

void *NPtrValue(NValue value) {
    switch (value.type) {
        case NTYPE_INT64 : return NULL;
        case NTYPE_UINT64: return NULL;
        case NTYPE_DOUBLE: return NULL;

        case NTYPE_CHAR8_PTR : return value.asPtr;
        case NTYPE_CHAR16_PTR: return value.asPtr;
        case NTYPE_CHAR32_PTR: return value.asPtr;
        case NTYPE_VOID_PTR  : return value.asPtr;
        case NTYPE_OBJECT_REF: return value.asPtr;
        case NTYPE_STRING_REF: return value.asPtr;
        case NTYPE_LAMBDA_REF: return value.asPtr;

        default: return NULL;
    }
}

NObject *NObjectValue(NValue value) {RETURN_PTR(value, NTYPE_OBJECT_REF);}
NString *NStringValue(NValue value) {RETURN_PTR(value, NTYPE_STRING_REF);}
NLambda *NLambdaValue(NValue value) {RETURN_PTR(value, NTYPE_LAMBDA_REF);}
