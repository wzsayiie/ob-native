#include "nnumber.h"

#define NUMBER_VAL_FLAG_BOOL   ((_NOBJECT_VAL_FLAG << 1) | 0x11)
#define NUMBER_VAL_FLAG_INT8   ((_NOBJECT_VAL_FLAG << 1) | 0x12)
#define NUMBER_VAL_FLAG_INT16  ((_NOBJECT_VAL_FLAG << 1) | 0x13)
#define NUMBER_VAL_FLAG_INT32  ((_NOBJECT_VAL_FLAG << 1) | 0x14)
#define NUMBER_VAL_FLAG_UINT8  ((_NOBJECT_VAL_FLAG << 1) | 0x15)
#define NUMBER_VAL_FLAG_UINT16 ((_NOBJECT_VAL_FLAG << 1) | 0x16)
#define NUMBER_VAL_FLAG_UINT32 ((_NOBJECT_VAL_FLAG << 1) | 0x17)
#define NUMBER_VAL_FLAG_FLOAT  ((_NOBJECT_VAL_FLAG << 1) | 0x18)

nstruct(_NNumber4, {
    NType type;
    union {
        int32_t  asInt32 ;
        uint32_t asUInt32;
        float    asFloat ;
    };
});

nstruct(_NNumber8, {
    NType type;
    union {
        int64_t  asInt64 ;
        uint64_t asUInt64;
        double   asDouble;
    };
});

NNumber *NNumberCreateWithBool(bool raw) {
    return NULL;
}

NNumber *NNumberCreateWithInt(int32_t raw) {
    return NULL;
}

NNumber *NNumberCreateWithInt64(int64_t raw) {
    return NULL;
}

NNumber *NNumberCreateWithUInt(uint32_t raw) {
    return NULL;
}

NNumber *NNumberCreateWithUInt64(uint64_t raw) {
    return NULL;
}

NNumber *NNumberCreateWithFloat(float raw) {
    return NULL;
}

NNumber *NNumberCreateWithDouble(double raw) {
    return NULL;
}

NNumber *NNumberCopy(NNumber *that) {
    return NULL;
}

bool NNumberGetBool(NNumber *number) {
    return false;
}

int NNumberGetInt(NNumber *number) {
    return 0;
}

int64_t NNumberGetInt64(NNumber *number) {
    return 0;
}

unsigned NNumberGetUInt(NNumber *number) {
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
    return 0;
}
