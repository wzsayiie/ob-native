#include "nnumber.h"
#include <float.h>

#if NPTR_32

nenum(NumberType, {
    NT_INT16 ,
    NT_UINT16,
    NT_INT32 ,
    NT_UINT32,
    NT_INT64 ,
    NT_UINT64,
    NT_FLOAT ,
    NT_DOUBLE,
});

nstruct(NumberValue, {
    struct {
        int8_t     _PAD[2];
        NumberType numType;
        int8_t     objFlag;
    };
    struct {
        int16_t  asInt16 ;
        uint16_t asUInt16;
    };
    NNumber *asNumber;
});

static NumberValue MakeValue(void) {
    NumberValue value = {0};
    value.objFlag = _NOBJECT_VALUE_FLAG;
    return value;
}

nstruct(NumberObject32, {
    NumberType numType;
    union {
        int32_t  asInt32 ;
        uint32_t asUInt32;
        float    asFloat ;
    };
});

nstruct(NumberObject64, {
    NumberType numType;
    union {
        int64_t  asInt64 ;
        uint64_t asUInt64;
        double   asDouble;
    };
});

NNumber *NNumberCreateWithBool(bool     raw) {return NNumberCreateWithInt64 (raw);}
NNumber *NNumberCreateWithInt (int      raw) {return NNumberCreateWithInt64 (raw);}
NNumber *NNumberCreateWithUInt(unsigned raw) {return NNumberCreateWithUInt64(raw);}

NNumber *NNumberCreateWithInt64(int64_t raw) {
    if (INT16_MIN <= raw && raw <= INT16_MAX) {
        NumberValue value = MakeValue();
        value.numType = NT_INT16;
        value.asInt16 = (int16_t)raw;
        return value.asNumber;

    } else if (INT32_MIN <= raw && raw <= INT32_MAX) {
        NumberObject32 *object = NCreate(NumberObject32, NULL);
        object->numType = NT_INT32;
        object->asInt32 = (int32_t)raw;
        return (NNumber *)object;

    } else {
        NumberObject64 *object = NCreate(NumberObject64, NULL);
        object->numType = NT_INT64;
        object->asInt64 = raw;
        return (NNumber *)object;
    }
}

NNumber *NNumberCreateWithUInt64(uint64_t raw) {
    if (raw <= UINT16_MAX) {
        NumberValue value = MakeValue();
        value.numType  = NT_UINT16;
        value.asUInt16 = (uint16_t)raw;
        return value.asNumber;

    } else if (raw <= UINT32_MAX) {
        NumberObject32 *object = NCreate(NumberObject32, NULL);
        object->numType  = NT_UINT32;
        object->asUInt32 = (uint32_t)raw;
        return (NNumber *)object;

    } else {
        NumberObject64 *object = NCreate(NumberObject64, NULL);
        object->numType  = NT_UINT64;
        object->asUInt64 = raw;
        return (NNumber *)object;
    }
}

NNumber *NNumberCreateWithFloat(float raw) {
    NumberObject32 *object = NCreate(NumberObject32, NULL);
    object->numType = NT_FLOAT;
    object->asFloat = raw;
    return (NNumber *)object;
}

NNumber *NNumberCreateWithDouble(double raw) {
    NumberObject64 *object = NCreate(NumberObject64, NULL);
    object->numType  = NT_DOUBLE;
    object->asDouble = raw;
    return (NNumber *)object;
}

static NumberValue *AsValue(NNumber *number) {
    NumberValue value = {0};
    value.asNumber = number;
    
    if (value.objFlag == _NOBJECT_VALUE_FLAG) {
        return (NumberValue *)number;
    } else {
        return NULL;
    }
}

static NumberObject32 *AsObject32(NNumber *number) {
    NumberObject32 *object = (NumberObject32 *)number;
    NumberType type = object->numType;

    if (type == NT_INT32 || type == NT_UINT32 || type == NT_FLOAT) {
        return object;
    } else {
        return NULL;
    }
}

static NumberObject64 *AsObject64(NNumber *number) {
    NumberObject64 *object = (NumberObject64 *)number;
    NumberType type = object->numType;

    if (type == NT_INT64 || type == NT_UINT64 || type == NT_DOUBLE) {
        return object;
    } else {
        return NULL;
    }
}

NNumber *NNumberCopy(NNumber *that) {
    return NULL;
}

bool     NNumberGetBool(NNumber *number) {return (bool    )NNumberGetInt64 (number);}
int      NNumberGetInt (NNumber *number) {return (int     )NNumberGetInt64 (number);}
unsigned NNumberGetUInt(NNumber *number) {return (unsigned)NNumberGetUInt64(number);}

int64_t NNumberGetInt64(NNumber *number) {
    return 0;
}

uint64_t NNumberGetUInt64(NNumber *number) {
    return 0;
}

float NNumberGetFloat(NNumber *number) {
    return 0;
}

double NNumberGetDouble(NNumber *number) {
    return 0;
}

int NNumberCompare(NNumber *number, NNumber *that) {
    double a = NNumberGetDouble(number);
    double b = NNumberGetDouble(that);

    if (a != b) {
        return a > b ? 1 : -1;
    } else {
        return 0;
    }
}

#endif
