#include "npod.h"

int64_t NMakeBoolWord  (bool     v) {NWord w; w.asBool   = v; return w.asInt64;}
int64_t NMakeInt8Word  (int8_t   v) {NWord w; w.asInt8   = v; return w.asInt64;}
int64_t NMakeInt16Word (int16_t  v) {NWord w; w.asInt16  = v; return w.asInt64;}
int64_t NMakeInt32Word (int32_t  v) {NWord w; w.asInt32  = v; return w.asInt64;}
int64_t NMakeInt64Word (int64_t  v) {NWord w; w.asInt64  = v; return w.asInt64;}
int64_t NMakeIntWord   (int      v) {NWord w; w.asInt    = v; return w.asInt64;}
int64_t NMakeUInt8Word (uint8_t  v) {NWord w; w.asUInt8  = v; return w.asInt64;}
int64_t NMakeUInt16Word(uint16_t v) {NWord w; w.asUInt16 = v; return w.asInt64;}
int64_t NMakeUInt32Word(uint32_t v) {NWord w; w.asUInt32 = v; return w.asInt64;}
int64_t NMakeUInt64Word(uint64_t v) {NWord w; w.asUInt64 = v; return w.asInt64;}
int64_t NMakeUIntWord  (unsigned v) {NWord w; w.asUInt   = v; return w.asInt64;}
int64_t NMakeFloatWord (float    v) {NWord w; w.asFloat  = v; return w.asInt64;}
int64_t NMakeDoubleWord(double   v) {NWord w; w.asDouble = v; return w.asInt64;}
int64_t NMakePtrWord   (void    *v) {NWord w; w.asPtr    = v; return w.asInt64;}

bool     NBoolWord  (int64_t v) {NWord w; w.asInt64 = v; return w.asBool  ;}
int8_t   NInt8Word  (int64_t v) {NWord w; w.asInt64 = v; return w.asInt8  ;}
int16_t  NInt16Word (int64_t v) {NWord w; w.asInt64 = v; return w.asInt16 ;}
int32_t  NInt32Word (int64_t v) {NWord w; w.asInt64 = v; return w.asInt32 ;}
int64_t  NInt64Word (int64_t v) {NWord w; w.asInt64 = v; return w.asInt64 ;}
int      NIntWord   (int64_t v) {NWord w; w.asInt64 = v; return w.asInt   ;}
uint8_t  NUInt8Word (int64_t v) {NWord w; w.asInt64 = v; return w.asUInt8 ;}
uint16_t NUInt16Word(int64_t v) {NWord w; w.asInt64 = v; return w.asUInt16;}
uint32_t NUInt32Word(int64_t v) {NWord w; w.asInt64 = v; return w.asUInt32;}
uint64_t NUInt64Word(int64_t v) {NWord w; w.asInt64 = v; return w.asUInt64;}
unsigned NUIntWord  (int64_t v) {NWord w; w.asInt64 = v; return w.asUInt  ;}
float    NFloatWord (int64_t v) {NWord w; w.asInt64 = v; return w.asFloat ;}
double   NDoubleWord(int64_t v) {NWord w; w.asInt64 = v; return w.asDouble;}
void *   NPtrWord   (int64_t v) {NWord w; w.asInt64 = v; return w.asPtr   ;}

NValue NMakeBoolValue  (bool     v) {NValue r; r.type = NTypeBool  ; r.asBool   = v; return r;}
NValue NMakeInt64Value (int64_t  v) {NValue r; r.type = NTypeInt64 ; r.asInt64  = v; return r;}
NValue NMakeUInt64Value(uint64_t v) {NValue r; r.type = NTypeUInt64; r.asUInt64 = v; return r;}
NValue NMakeDoubleValue(double   v) {NValue r; r.type = NTypeDouble; r.asDouble = v; return r;}
NValue NMakePtrValue   (void    *v) {NValue r; r.type = NTypePtr   ; r.asPtr    = v; return r;}

NValue NMakeIntValue   (int      v) {return NMakeInt64Value (v);}
NValue NMakeUIntValue  (unsigned v) {return NMakeUInt64Value(v);}
NValue NMakeFloatValue (float    v) {return NMakeDoubleValue(v);}

bool NBoolValue(NValue value) {
    switch (value.type) {
        case NTypeBool  : return value.asBool  ;
        case NTypeInt64 : return value.asInt64 ;
        case NTypeUInt64: return value.asUInt64;
        case NTypeDouble: return value.asDouble;
        case NTypePtr   : return value.asPtr   ;
        default/*error*/: return false         ;
    }
}

int64_t NInt64Value(NValue value) {
    switch (value.type) {
        case NTypeBool  : return (int64_t)value.asBool  ;
        case NTypeInt64 : return (int64_t)value.asInt64 ;
        case NTypeUInt64: return (int64_t)value.asUInt64;
        case NTypeDouble: return (int64_t)value.asDouble;
        case NTypePtr   : return (int64_t)value.asPtr   ;
        default/*error*/: return (int64_t)0             ;
    }
}

uint64_t NUInt64Value(NValue value) {
    switch (value.type) {
        case NTypeBool  : return (uint64_t)value.asBool  ;
        case NTypeInt64 : return (uint64_t)value.asInt64 ;
        case NTypeUInt64: return (uint64_t)value.asUInt64;
        case NTypeDouble: return (uint64_t)value.asDouble;
        case NTypePtr   : return (uint64_t)value.asPtr   ;
        default/*error*/: return (uint64_t)0             ;
    }
}

double NDoubleValue(NValue value) {
    switch (value.type) {
        case NTypeBool  : return (double)value.asBool  ;
        case NTypeInt64 : return (double)value.asInt64 ;
        case NTypeUInt64: return (double)value.asUInt64;
        case NTypeDouble: return (double)value.asDouble;
        case NTypePtr   : return (double)0             ;
        default/*error*/: return (double)0             ;
    }
}

void *NPtrValue(NValue value) {
    if (value.type == NTypePtr) {
        return value.asPtr;
    } else {
        return NULL;
    }
}

int      NIntValue  (NValue v) {return (int     )NInt64Value (v);}
unsigned NUIntValue (NValue v) {return (unsigned)NUInt64Value(v);}
float    NFloatValue(NValue v) {return (float   )NDoubleValue(v);}
