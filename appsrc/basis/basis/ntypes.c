#include "ntypes.h"

NValue NMakeBoolValue(bool value) {
    NValue ret;
    ret.boolValue = value;
    ret.type = NTypeBool;
    return ret;
}

NValue NMakeIntValue(int64_t value) {
    NValue ret;
    ret.intValue = value;
    ret.type = NTypeInt64;
    return ret;
}

NValue NMakeUIntValue(uint64_t value) {
    NValue ret;
    ret.uintValue = value;
    ret.type = NTypeUInt64;
    return ret;
}

NValue NMakeDblValue(double value) {
    NValue ret;
    ret.dblValue = value;
    ret.type = NTypeDouble;
    return ret;
}

NValue NMakePtrValue(void *value) {
    NValue ret;
    ret.ptrValue = value;
    ret.type = NTypePtr;
    return ret;
}

bool NBoolValue(NValue value) {
    switch (value.type) {
        case NTypeBool  : return /*..*/value.boolValue;
        case NTypeInt8  : return (bool)value.intValue ;
        case NTypeInt16 : return (bool)value.intValue ;
        case NTypeInt32 : return (bool)value.intValue ;
        case NTypeInt64 : return (bool)value.intValue ;
        case NTypeUInt8 : return (bool)value.uintValue;
        case NTypeUInt16: return (bool)value.uintValue;
        case NTypeUInt32: return (bool)value.uintValue;
        case NTypeUInt64: return (bool)value.uintValue;
        case NTypeFloat : return (bool)value.dblValue ;
        case NTypeDouble: return (bool)value.dblValue ;
        case NTypePtr   : return (bool)value.ptrValue ;
        default/*other*/: return false;
    }
}

int64_t NIntValue(NValue value) {
    switch (value.type) {
        case NTypeBool  : return (int64_t)value.boolValue;
        case NTypeInt8  : return /*.....*/value.intValue ;
        case NTypeInt16 : return /*.....*/value.intValue ;
        case NTypeInt32 : return /*.....*/value.intValue ;
        case NTypeInt64 : return /*.....*/value.intValue ;
        case NTypeUInt8 : return (int64_t)value.uintValue;
        case NTypeUInt16: return (int64_t)value.uintValue;
        case NTypeUInt32: return (int64_t)value.uintValue;
        case NTypeUInt64: return (int64_t)value.uintValue;
        case NTypeFloat : return (int64_t)value.dblValue ;
        case NTypeDouble: return (int64_t)value.dblValue ;
        case NTypePtr   : return (int64_t)value.ptrValue ;
        default/*other*/: return 0;
    }
}

uint64_t NUIntValue(NValue value) {
    switch (value.type) {
        case NTypeBool  : return (uint64_t)value.boolValue;
        case NTypeInt8  : return (uint64_t)value.intValue ;
        case NTypeInt16 : return (uint64_t)value.intValue ;
        case NTypeInt32 : return (uint64_t)value.intValue ;
        case NTypeInt64 : return (uint64_t)value.intValue ;
        case NTypeUInt8 : return /*......*/value.uintValue;
        case NTypeUInt16: return /*......*/value.uintValue;
        case NTypeUInt32: return /*......*/value.uintValue;
        case NTypeUInt64: return /*......*/value.uintValue;
        case NTypeFloat : return (uint64_t)value.dblValue ;
        case NTypeDouble: return (uint64_t)value.dblValue ;
        case NTypePtr   : return (uint64_t)value.ptrValue ;
        default/*other*/: return 0;
    }
}

double NDblValue(NValue value) {
    switch (value.type) {
        case NTypeBool  : return (double)value.boolValue;
        case NTypeInt8  : return (double)value.intValue ;
        case NTypeInt16 : return (double)value.intValue ;
        case NTypeInt32 : return (double)value.intValue ;
        case NTypeInt64 : return (double)value.intValue ;
        case NTypeUInt8 : return (double)value.uintValue;
        case NTypeUInt16: return (double)value.uintValue;
        case NTypeUInt32: return (double)value.uintValue;
        case NTypeUInt64: return (double)value.uintValue;
        case NTypeFloat : return /*....*/value.dblValue ;
        case NTypeDouble: return /*....*/value.dblValue ;
        case NTypePtr   : return 0;
        default/*other*/: return 0;
    }
}

void *NPtrValue(NValue value) {
    switch (value.type) {
        case NTypeBool  : return NULL;
        case NTypeInt8  : return (void *)value.intValue ;
        case NTypeInt16 : return (void *)value.intValue ;
        case NTypeInt32 : return (void *)value.intValue ;
        case NTypeInt64 : return (void *)value.intValue ;
        case NTypeUInt8 : return (void *)value.uintValue;
        case NTypeUInt16: return (void *)value.uintValue;
        case NTypeUInt32: return (void *)value.uintValue;
        case NTypeUInt64: return (void *)value.uintValue;
        case NTypeFloat : return NULL;
        case NTypeDouble: return NULL;
        case NTypePtr   : return value.ptrValue;
        default/*other*/: return NULL;
    }
}
