#include "nnumber.h"

#define INIT(NUM, VAL) _NObjectInit(nsuperof(NUM)); NUM->value = VAL;

void _NNumberInitWithBool  (NNumber *num, bool     raw) {INIT(num, NMakeBoolValue  (raw));}
void _NNumberInitWithInt64 (NNumber *num, int64_t  raw) {INIT(num, NMakeInt64Value (raw));}
void _NNumberInitWithUInt64(NNumber *num, uint64_t raw) {INIT(num, NMakeUInt64Value(raw));}
void _NNumberInitWithDouble(NNumber *num, double   raw) {INIT(num, NMakeDoubleValue(raw));}
void _NNumberInitWithInt   (NNumber *num, int      raw) {INIT(num, NMakeIntValue   (raw));}
void _NNumberInitWithUInt  (NNumber *num, unsigned raw) {INIT(num, NMakeUIntValue  (raw));}
void _NNumberInitWithFloat (NNumber *num, float    raw) {INIT(num, NMakeFloatValue (raw));}
void _NNumberInit          (NNumber *num              ) {INIT(num, NMakeIntValue   (0)  );}

void _NNumberDeinit(NNumber *number) {
    _NObjectDeinit(nsuperof(number));
}

#define RETURN_CREATE(INIT, ...)                            \
/**/    NNumber *number = NAlloc(NNumber, _NNumberDeinit);  \
/**/    INIT(number, ##__VA_ARGS__);                        \
/**/    return number;

NNumber *NNumberCreateWithBool  (bool     raw) {RETURN_CREATE(_NNumberInitWithBool  , raw);}
NNumber *NNumberCreateWithInt64 (int64_t  raw) {RETURN_CREATE(_NNumberInitWithInt64 , raw);}
NNumber *NNumberCreateWithUInt64(uint64_t raw) {RETURN_CREATE(_NNumberInitWithUInt64, raw);}
NNumber *NNumberCreateWithDouble(double   raw) {RETURN_CREATE(_NNumberInitWithDouble, raw);}
NNumber *NNumberCreateWithInt   (int      raw) {RETURN_CREATE(_NNumberInitWithInt   , raw);}
NNumber *NNumberCreateWithUInt  (unsigned raw) {RETURN_CREATE(_NNumberInitWithUInt  , raw);}
NNumber *NNumberCreateWithFloat (float    raw) {RETURN_CREATE(_NNumberInitWithFloat , raw);}
NNumber *NNumberCreate          (void        ) {RETURN_CREATE(_NNumberInit               );}

NNumber *NNumberCopy(NNumber *that) {
    if (that) {
        NNumber *number = NNumberCreate();
        number->value = that->value;
        return number;
    }
    return NULL;
}

void NNumberSetBool  (NNumber *num, bool     raw) {if (num) {num->value = NMakeBoolValue  (raw);}}
void NNumberSetInt   (NNumber *num, int      raw) {if (num) {num->value = NMakeIntValue   (raw);}}
void NNumberSetInt64 (NNumber *num, int64_t  raw) {if (num) {num->value = NMakeInt64Value (raw);}}
void NNumberSetUInt  (NNumber *num, unsigned raw) {if (num) {num->value = NMakeUIntValue  (raw);}}
void NNumberSetUInt64(NNumber *num, uint64_t raw) {if (num) {num->value = NMakeUInt64Value(raw);}}
void NNumberSetFloat (NNumber *num, float    raw) {if (num) {num->value = NMakeFloatValue (raw);}}
void NNumberSetDouble(NNumber *num, double   raw) {if (num) {num->value = NMakeDoubleValue(raw);}}

bool     NNumberGetBool  (NNumber *num) {return num ? NBoolValue  (num->value) : 0;}
int      NNumberGetInt   (NNumber *num) {return num ? NIntValue   (num->value) : 0;}
int64_t  NNumberGetInt64 (NNumber *num) {return num ? NInt64Value (num->value) : 0;}
unsigned NNumberGetUInt  (NNumber *num) {return num ? NUIntValue  (num->value) : 0;}
uint64_t NNumberGetUInt64(NNumber *num) {return num ? NUInt64Value(num->value) : 0;}
float    NNumberGetFloat (NNumber *num) {return num ? NFloatValue (num->value) : 0;}
double   NNumberGetDouble(NNumber *num) {return num ? NDoubleValue(num->value) : 0;}

int NNumberCompare(NNumber *number, NNumber *that) {
    double a = NNumberGetDouble(number);
    double b = NNumberGetDouble(that);
    
    if (a > b) {return  1;}
    if (a < b) {return -1;}
    else /***/ {return  0;}
}
