#include "npod.h"
#include "nstring.h"

NValue NMakeVoidValue(void) {
    NValue value = {0};
    return value;
}

#define MAKE(TYPE, MEM, RAW) NValue val = {0}; val.type = TYPE; val.MEM = RAW; return val

NValue NMakeBoolValue  (bool     raw) {MAKE(NTYPE_INT64   , asInt64 , raw);}
NValue NMakeIntValue   (int      raw) {MAKE(NTYPE_INT64   , asInt64 , raw);}
NValue NMakeInt64Value (int64_t  raw) {MAKE(NTYPE_INT64   , asInt64 , raw);}
NValue NMakeUIntValue  (unsigned raw) {MAKE(NTYPE_UINT64  , asUInt64, raw);}
NValue NMakeUInt64Value(uint64_t raw) {MAKE(NTYPE_UINT64  , asUInt64, raw);}
NValue NMakeFloatValue (float    raw) {MAKE(NTYPE_DOUBLE  , asDouble, raw);}
NValue NMakeDoubleValue(double   raw) {MAKE(NTYPE_DOUBLE  , asDouble, raw);}
NValue NMakePtrValue   (void    *raw) {MAKE(NTYPE_VOID_PTR, asPtr   , raw);}

NValue NMakeU8CharsValue (const char     *raw) {MAKE(NTYPE_CHAR8_PTR , asPtr, (char     *)raw);}
NValue NMakeU16CharsValue(const char16_t *raw) {MAKE(NTYPE_CHAR16_PTR, asPtr, (char16_t *)raw);}
NValue NMakeU32CharsValue(const char32_t *raw) {MAKE(NTYPE_CHAR32_PTR, asPtr, (char32_t *)raw);}

NValue NMakeObjectValue(void    *raw) {MAKE(NTYPE_OBJECT, asObject, raw);}
NValue NMakeStringValue(NString *raw) {MAKE(NTYPE_STRING, asObject, raw);}
NValue NMakeLambdaValue(NLambda *raw) {MAKE(NTYPE_LAMBDA, asObject, raw);}

bool NBoolValue(NValue value) {
    switch (value.type) {
        case NTYPE_INT64 : return value.asInt64  != 0;
        case NTYPE_UINT64: return value.asUInt64 != 0;
        case NTYPE_DOUBLE: return value.asDouble != 0;

        case NTYPE_VOID_PTR  : return value.asPtr != 0;
        case NTYPE_CHAR8_PTR : return value.asPtr != 0;
        case NTYPE_CHAR16_PTR: return value.asPtr != 0;
        case NTYPE_CHAR32_PTR: return value.asPtr != 0;
        
        case NTYPE_OBJECT: return value.asObject != 0;
        case NTYPE_STRING: return value.asObject != 0;
        case NTYPE_LAMBDA: return value.asObject != 0;

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
        case NTYPE_VOID_PTR  : return (int64_t)value.asPtr;
        case NTYPE_CHAR8_PTR : return (int64_t)value.asPtr;
        case NTYPE_CHAR16_PTR: return (int64_t)value.asPtr;
        case NTYPE_CHAR32_PTR: return (int64_t)value.asPtr;
        
        case NTYPE_OBJECT: return (int64_t)value.asObject;
        case NTYPE_STRING: return (int64_t)value.asObject;
        case NTYPE_LAMBDA: return (int64_t)value.asObject;

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
        case NTYPE_VOID_PTR  : return (uint64_t)value.asPtr;
        case NTYPE_CHAR8_PTR : return (uint64_t)value.asPtr;
        case NTYPE_CHAR16_PTR: return (uint64_t)value.asPtr;
        case NTYPE_CHAR32_PTR: return (uint64_t)value.asPtr;
        
        case NTYPE_OBJECT: return (uint64_t)value.asObject;
        case NTYPE_STRING: return (uint64_t)value.asObject;
        case NTYPE_LAMBDA: return (uint64_t)value.asObject;

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

        default: return 0;
    }
}

void *NPtrValue(NValue value) {
    switch (value.type) {
        case NTYPE_VOID_PTR  : return value.asPtr;
        case NTYPE_CHAR8_PTR : return value.asPtr;
        case NTYPE_CHAR16_PTR: return value.asPtr;
        case NTYPE_CHAR32_PTR: return value.asPtr;

        //object reference also is a pointer.
        case NTYPE_OBJECT: return value.asObject;
        case NTYPE_STRING: return value.asObject;
        case NTYPE_LAMBDA: return value.asObject;

        default: return NULL;
    }
}

char *NU8CharsValue(NValue value) {
    if (value.type == NTYPE_STRING) {
        return (char *)NStringU8Chars(value.asObject);
    }
    if (value.type == NTYPE_CHAR8_PTR) {
        return value.asPtr;
    }
    return NULL;
}

char16_t *NU16CharsValue(NValue value) {
    if (value.type == NTYPE_STRING) {
        return (char16_t *)NStringU16Chars(value.asObject);
    }
    if (value.type == NTYPE_CHAR16_PTR) {
        return value.asPtr;
    }
    return NULL;
}

char32_t *NU32CharsValue(NValue value) {
    if (value.type == NTYPE_STRING) {
        return (char32_t *)NStringU32Chars(value.asObject);
    }
    if (value.type == NTYPE_CHAR32_PTR) {
        return value.asPtr;
    }
    return NULL;
}

void *NObjectValue(NValue value) {
    switch (value.type) {
        case NTYPE_OBJECT: return value.asObject;
        case NTYPE_STRING: return value.asObject;
        case NTYPE_LAMBDA: return value.asObject;

        default: return NULL;
    }
}

NString *NStringValue(NValue value) {
    if (value.type == NTYPE_STRING) {
        return value.asObject;
    }
    return NULL;
}

NLambda *NLambdaValue(NValue value) {
    if (value.type == NTYPE_LAMBDA) {
        return value.asObject;
    }
    return NULL;
}
