#include "nnumber.h"
#include "npod.h"

nstruct(NNumber) {
    NValue value;
};

NNumber *NNumberCreateWithBool  (bool     v) {NNumber *s = NNumberCreate(); s->value = NMakeBoolValue  (v); return s;}
NNumber *NNumberCreateWithInt64 (int64_t  v) {NNumber *s = NNumberCreate(); s->value = NMakeInt64Value (v); return s;}
NNumber *NNumberCreateWithUInt64(uint64_t v) {NNumber *s = NNumberCreate(); s->value = NMakeUInt64Value(v); return s;}
NNumber *NNumberCreateWithDouble(double   v) {NNumber *s = NNumberCreate(); s->value = NMakeDoubleValue(v); return s;}
NNumber *NNumberCreateWithInt   (int      v) {NNumber *s = NNumberCreate(); s->value = NMakeIntValue   (v); return s;}
NNumber *NNumberCreateWithUInt  (unsigned v) {NNumber *s = NNumberCreate(); s->value = NMakeUIntValue  (v); return s;}
NNumber *NNumberCreateWithFloat (float    v) {NNumber *s = NNumberCreate(); s->value = NMakeFloatValue (v); return s;}

NNumber *NNumberCreate(void) {
    return NCreate(nisizeof(NNumber), NULL);
}

NNumber *NNumberCopy(NNumber *that) {
    if (that) {
        NNumber *copy = NNumberCreate();
        copy->value = that->value;
        return copy;
    }
    return NULL;
}

void NNumberSetBool  (NNumber *s, bool     v) {if (s) {s->value = NMakeBoolValue  (v);}}
void NNumberSetInt64 (NNumber *s, int64_t  v) {if (s) {s->value = NMakeInt64Value (v);}}
void NNumberSetUInt64(NNumber *s, uint64_t v) {if (s) {s->value = NMakeUInt64Value(v);}}
void NNumberSetDouble(NNumber *s, double   v) {if (s) {s->value = NMakeDoubleValue(v);}}
void NNumberSetInt   (NNumber *s, int      v) {if (s) {s->value = NMakeIntValue   (v);}}
void NNumberSetUInt  (NNumber *s, unsigned v) {if (s) {s->value = NMakeUIntValue  (v);}}
void NNumberSetFloat (NNumber *s, float    v) {if (s) {s->value = NMakeFloatValue (v);}}

bool     NNumberGetBool  (NNumber *s) {return s ? NBoolValue  (s->value) : 0;}
int64_t  NNumberGetInt64 (NNumber *s) {return s ? NInt64Value (s->value) : 0;}
uint64_t NNumberGetUInt64(NNumber *s) {return s ? NUInt64Value(s->value) : 0;}
double   NNumberGetDouble(NNumber *s) {return s ? NDoubleValue(s->value) : 0;}
int      NNumberGetInt   (NNumber *s) {return s ? NIntValue   (s->value) : 0;}
unsigned NNumberGetUInt  (NNumber *s) {return s ? NUIntValue  (s->value) : 0;}
float    NNumberGetFloat (NNumber *s) {return s ? NFloatValue (s->value) : 0;}

int NNumberCompare(NNumber *self, NNumber *that) {
    double a = NNumberGetDouble(self);
    double b = NNumberGetDouble(that);
    
    if (a > b) {return  1;}
    if (a < b) {return -1;}
    else /***/ {return  0;}
}
