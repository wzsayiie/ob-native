#include "nnumber.h"

#define GEN_INIT(NUM, VAL) _NObjectInit(&NUM->Super); NUM->value = VAL;

void _NNumberInitWithBool  (NNumber *num, bool     val) {GEN_INIT(num, NMakeBoolValue  (val));}
void _NNumberInitWithInt64 (NNumber *num, int64_t  val) {GEN_INIT(num, NMakeInt64Value (val));}
void _NNumberInitWithUInt64(NNumber *num, uint64_t val) {GEN_INIT(num, NMakeUInt64Value(val));}
void _NNumberInitWithDouble(NNumber *num, double   val) {GEN_INIT(num, NMakeDoubleValue(val));}
void _NNumberInitWithInt   (NNumber *num, int      val) {GEN_INIT(num, NMakeIntValue   (val));}
void _NNumberInitWithUInt  (NNumber *num, unsigned val) {GEN_INIT(num, NMakeUIntValue  (val));}
void _NNumberInitWithFloat (NNumber *num, float    val) {GEN_INIT(num, NMakeFloatValue (val));}
void _NNumberInit          (NNumber *num              ) {GEN_INIT(num, NMakeIntValue   (0)  );}

void _NNumberDeinit(NNumber *number) {
    _NObjectDeinit(&number->Super);
}

#define GEN_CREATE(I, ...) NNumber *n = NAlloc(NNumber, _NNumberDeinit); I(n, ##__VA_ARGS__); return n;

NNumber *NNumberCreateWithBool  (bool     val) {GEN_CREATE(_NNumberInitWithBool  , val);}
NNumber *NNumberCreateWithInt64 (int64_t  val) {GEN_CREATE(_NNumberInitWithInt64 , val);}
NNumber *NNumberCreateWithUInt64(uint64_t val) {GEN_CREATE(_NNumberInitWithUInt64, val);}
NNumber *NNumberCreateWithDouble(double   val) {GEN_CREATE(_NNumberInitWithDouble, val);}
NNumber *NNumberCreateWithInt   (int      val) {GEN_CREATE(_NNumberInitWithInt   , val);}
NNumber *NNumberCreateWithUInt  (unsigned val) {GEN_CREATE(_NNumberInitWithUInt  , val);}
NNumber *NNumberCreateWithFloat (float    val) {GEN_CREATE(_NNumberInitWithFloat , val);}
NNumber *NNumberCreate          (void        ) {GEN_CREATE(_NNumberInit               );}

NNumber *NNumberCopy(NNumber *that) {
    if (that) {
        NNumber *copy = NNumberCreate();
        copy->value = that->value;
        return copy;
    }
    return NULL;
}

void NNumberSetBool  (NNumber *num, bool     val) {if (num) {num->value = NMakeBoolValue  (val);}}
void NNumberSetInt64 (NNumber *num, int64_t  val) {if (num) {num->value = NMakeInt64Value (val);}}
void NNumberSetUInt64(NNumber *num, uint64_t val) {if (num) {num->value = NMakeUInt64Value(val);}}
void NNumberSetDouble(NNumber *num, double   val) {if (num) {num->value = NMakeDoubleValue(val);}}
void NNumberSetInt   (NNumber *num, int      val) {if (num) {num->value = NMakeIntValue   (val);}}
void NNumberSetUInt  (NNumber *num, unsigned val) {if (num) {num->value = NMakeUIntValue  (val);}}
void NNumberSetFloat (NNumber *num, float    val) {if (num) {num->value = NMakeFloatValue (val);}}

bool     NNumberGetBool  (NNumber *num) {return num ? NBoolValue  (num->value) : 0;}
int64_t  NNumberGetInt64 (NNumber *num) {return num ? NInt64Value (num->value) : 0;}
uint64_t NNumberGetUInt64(NNumber *num) {return num ? NUInt64Value(num->value) : 0;}
double   NNumberGetDouble(NNumber *num) {return num ? NDoubleValue(num->value) : 0;}
int      NNumberGetInt   (NNumber *num) {return num ? NIntValue   (num->value) : 0;}
unsigned NNumberGetUInt  (NNumber *num) {return num ? NUIntValue  (num->value) : 0;}
float    NNumberGetFloat (NNumber *num) {return num ? NFloatValue (num->value) : 0;}

int NNumberCompare(NNumber *number, NNumber *that) {
    double a = NNumberGetDouble(number);
    double b = NNumberGetDouble(that);
    
    if (a > b) {return  1;}
    if (a < b) {return -1;}
    else /***/ {return  0;}
}
