#include "nnumber.h"
#include <float.h>

#if NPTR_32

nenum(NumberType, {
    NT_INT16  = 1,
    NT_UINT16 = 2,

    NT_INT32  = 3,
    NT_UINT32 = 4,
    NT_FLOAT  = 5,

    NT_INT64  = 6,
    NT_UINT64 = 7,
    NT_DOUBLE = 8,
});

nunion(NumberWord, {
    int16_t  asInt16 ;
    uint16_t asUInt16;
    
    int32_t  asInt32 ;
    uint32_t asUInt32;
    float    asFloat ;
    
    int64_t  asInt64 ;
    uint64_t asUInt64;
    double   asDouble;
});

static bool Is16Bits(NumberType type) {
    switch (type) {
        case NT_INT16 : return true;
        case NT_UINT16: return true;
        default:;
    }
    return false;
}

static bool Is32Bits(NumberType type) {
    switch (type) {
        case NT_INT32 : return true;
        case NT_UINT32: return true;
        case NT_FLOAT : return true;
        default:;
    }
    return false;
}

nunion(NumberValue, {
    struct {
        int16_t store16;
        int8_t  numType;
        int8_t  objFlag;
    };
    NNumber *asNumber;
});

nstruct(NumberObject32, {
    int32_t numType;
    int32_t store32;
});

nstruct(NumberObject64, {
    int32_t numType;
    int32_t _FILL32;
    int64_t store64;
});

static NNumber *NumberCreate(NumberType type, NumberWord word) {
    if (Is16Bits(type)) {
        NumberValue value = {
            .objFlag = _NOBJECT_VALUE_FLAG,
            .numType = type,
            .store16 = word.asInt16,
        };
        return value.asNumber;
        
    } else if (Is32Bits(type)) {
        NumberObject32 *object = NCreate(NumberObject32, NULL);
        object->numType = type;
        object->store32 = word.asInt32;
        return (NNumber *)object;
        
    } else {
        NumberObject64 *object = NCreate(NumberObject64, NULL);
        object->numType = type;
        object->store64 = word.asInt64;
        return (NNumber *)object;
    }
}

NNumber *NNumberCreateWithBool(bool raw) {return NNumberCreateWithInt64 (raw);}
NNumber *NNumberCreateWithInt (int  raw) {return NNumberCreateWithInt64 (raw);}
NNumber *NNumberCreateWithUInt(uint raw) {return NNumberCreateWithUInt64(raw);}

NNumber *NNumberCreateWithInt64(int64_t raw) {
    if (INT16_MIN <= raw && raw <= INT16_MAX) {
        NumberWord word = {
            .asInt16 = (int16_t)raw
        };
        return NumberCreate(NT_INT16, word);

    } else if (INT32_MIN <= raw && raw <= INT32_MAX) {
        NumberWord word = {
            .asInt32 = (int32_t)raw
        };
        return NumberCreate(NT_INT32, word);

    } else {
        NumberWord word = {
            .asInt64 = raw
        };
        return NumberCreate(NT_INT64, word);
    }
}

NNumber *NNumberCreateWithUInt64(uint64_t raw) {
    if (raw <= UINT16_MAX) {
        NumberWord word = {
            .asUInt16 = (uint16_t)raw
        };
        return NumberCreate(NT_UINT16, word);

    } else if (raw <= UINT32_MAX) {
        NumberWord word = {
            .asUInt32 = (uint32_t)raw
        };
        return NumberCreate(NT_UINT32, word);

    } else {
        NumberWord word = {
            .asUInt64 = raw
        };
        return NumberCreate(NT_UINT64, word);
    }
}

NNumber *NNumberCreateWithFloat(float raw) {
    NumberWord word = {
        .asFloat = raw
    };
    return NumberCreate(NT_FLOAT, word);
}

NNumber *NNumberCreateWithDouble(double raw) {
    NumberWord word = {
        .asDouble = raw
    };
    return NumberCreate(NT_DOUBLE, word);
}

static void NumberGetWord(NNumber *number, NumberType *type, NumberWord *word) {
    NumberValue value = {
        .asNumber = number
    };
    if (value.objFlag == _NOBJECT_VALUE_FLAG) {
        *type = value.numType;
        word->asInt16 = value.store16;
        return;
    }
    
    NumberObject32 *object32 = (NumberObject32 *)number;
    if (Is32Bits(object32->numType)) {
        *type = object32->numType;
        word->asInt32 = object32->store32;
        return;
    }
    
    NumberObject64 *object64 = (NumberObject64 *)number;
    *type = object64->numType;
    word->asInt64 = object64->store64;
}

NNumber *NNumberCopy(NNumber *that) {
    if (!that) {
        return NULL;
    }
    
    NumberType type = 0;
    NumberWord word = {0};
    NumberGetWord(that, &type, &word);
    
    return NumberCreate(type, word);
}

bool  NNumberGetBool (NNumber *num) {return (bool )NNumberGetInt64 (num);}
int   NNumberGetInt  (NNumber *num) {return (int  )NNumberGetInt64 (num);}
uint  NNumberGetUInt (NNumber *num) {return (uint )NNumberGetUInt64(num);}
float NNumberGetFloat(NNumber *num) {return (float)NNumberGetDouble(num);}

int64_t NNumberGetInt64(NNumber *number) {
    if (!number) {
        return 0;
    }
    
    NumberType type = 0;
    NumberWord word = {0};
    NumberGetWord(number, &type, &word);
    
    switch (type) {
        case NT_INT16 : return (int64_t)word.asInt16 ;
        case NT_UINT16: return (int64_t)word.asUInt16;
        case NT_INT32 : return (int64_t)word.asInt32 ;
        case NT_UINT32: return (int64_t)word.asUInt32;
        case NT_FLOAT : return (int64_t)word.asFloat ;
        case NT_INT64 : return (int64_t)word.asInt64 ;
        case NT_UINT64: return (int64_t)word.asUInt64;
        case NT_DOUBLE: return (int64_t)word.asDouble;
        default: return 0;
    }
}

uint64_t NNumberGetUInt64(NNumber *number) {
    if (!number) {
        return 0;
    }
    
    NumberType type = 0;
    NumberWord word = {0};
    NumberGetWord(number, &type, &word);
    
    switch (type) {
        case NT_INT16 : return (uint64_t)word.asInt16 ;
        case NT_UINT16: return (uint64_t)word.asUInt16;
        case NT_INT32 : return (uint64_t)word.asInt32 ;
        case NT_UINT32: return (uint64_t)word.asUInt32;
        case NT_FLOAT : return (uint64_t)word.asFloat ;
        case NT_INT64 : return (uint64_t)word.asInt64 ;
        case NT_UINT64: return (uint64_t)word.asUInt64;
        case NT_DOUBLE: return (uint64_t)word.asDouble;
        default: return 0;
    }
}

double NNumberGetDouble(NNumber *number) {
    if (!number) {
        return 0;
    }
    
    NumberType type = 0;
    NumberWord word = {0};
    NumberGetWord(number, &type, &word);
    
    switch (type) {
        case NT_INT16 : return (double)word.asInt16 ;
        case NT_UINT16: return (double)word.asUInt16;
        case NT_INT32 : return (double)word.asInt32 ;
        case NT_UINT32: return (double)word.asUInt32;
        case NT_FLOAT : return (double)word.asFloat ;
        case NT_INT64 : return (double)word.asInt64 ;
        case NT_UINT64: return (double)word.asUInt64;
        case NT_DOUBLE: return (double)word.asDouble;
        default: return 0;
    }
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
