#include "ntypes.h"

NValue NMakeBoolValue  (bool     v) {NValue r; r.type = NTypeBool  ; r.asBool   = v; return r;}
NValue NMakeInt64Value (int64_t  v) {NValue r; r.type = NTypeInt64 ; r.asInt64  = v; return r;}
NValue NMakeUInt64Value(uint64_t v) {NValue r; r.type = NTypeUInt64; r.asUInt64 = v; return r;}
NValue NMakeDoubleValue(double   v) {NValue r; r.type = NTypeDouble; r.asDouble = v; return r;}
NValue NMakePtrValue   (void    *v) {NValue r; r.type = NTypePtr   ; r.asPtr    = v; return r;}

NValue NMakeIntValue   (int      value) {return NMakeInt64Value (value);}
NValue NMakeUIntValue  (unsigned value) {return NMakeUInt64Value(value);}
NValue NMakeFloatValue (float    value) {return NMakeDoubleValue(value);}

bool _NBoolValue(NValue value, bool *cast) {
    switch (value.type) {
        case NTypeBool  : *cast = true ; return value.asBool  ;
        case NTypeInt64 : *cast = true ; return value.asInt64 ;
        case NTypeUInt64: *cast = true ; return value.asUInt64;
        case NTypeDouble: *cast = true ; return value.asDouble;
        case NTypePtr   : *cast = true ; return value.asPtr   ;
        default/*error*/: *cast = false; return false         ;
    }
}

int64_t _NInt64Value(NValue value, bool *cast) {
    switch (value.type) {
        case NTypeBool  : *cast = true ; return (int64_t)value.asBool  ;
        case NTypeInt64 : *cast = true ; return (int64_t)value.asInt64 ;
        case NTypeUInt64: *cast = true ; return (int64_t)value.asUInt64;
        case NTypeDouble: *cast = true ; return (int64_t)value.asDouble;
        case NTypePtr   : *cast = true ; return (int64_t)value.asPtr   ;
        default/*error*/: *cast = false; return (int64_t)0             ;
    }
}

uint64_t _NUInt64Value(NValue value, bool *cast) {
    switch (value.type) {
        case NTypeBool  : *cast = true ; return (uint64_t)value.asBool  ;
        case NTypeInt64 : *cast = true ; return (uint64_t)value.asInt64 ;
        case NTypeUInt64: *cast = true ; return (uint64_t)value.asUInt64;
        case NTypeDouble: *cast = true ; return (uint64_t)value.asDouble;
        case NTypePtr   : *cast = true ; return (uint64_t)value.asPtr   ;
        default/*error*/: *cast = false; return (uint64_t)0             ;
    }
}

double _NDoubleValue(NValue value, bool *cast) {
    switch (value.type) {
        case NTypeBool  : *cast = true ; return (double)value.asBool  ;
        case NTypeInt64 : *cast = true ; return (double)value.asInt64 ;
        case NTypeUInt64: *cast = true ; return (double)value.asUInt64;
        case NTypeDouble: *cast = true ; return (double)value.asDouble;
        case NTypePtr   : *cast = false; return (double)0             ;
        default/*error*/: *cast = false; return (double)0             ;
    }
}

void *_NPtrValue(NValue value, bool *cast) {
    if (value.type == NTypePtr) {
        *cast = true;
        return value.asPtr;
    } else {
        *cast = false;
        return NULL;
    }
}

bool NValueCastable(NValue value, NType type) {
    bool cast = false;
    switch (value.type) {
        case NTypeBool  : _NBoolValue  (value, &cast); break;
        case NTypeInt64 : _NInt64Value (value, &cast); break;
        case NTypeUInt64: _NUInt64Value(value, &cast); break;
        case NTypeDouble: _NDoubleValue(value, &cast); break;
        case NTypePtr   : _NPtrValue   (value, &cast); break;
        default/*error*/:;
    }
    return cast;
}

bool     NBoolValue  (NValue value) {bool _; return _NBoolValue  (value, &_);}
int64_t  NInt64Value (NValue value) {bool _; return _NInt64Value (value, &_);}
uint64_t NUInt64Value(NValue value) {bool _; return _NUInt64Value(value, &_);}
double   NDoubleValue(NValue value) {bool _; return _NDoubleValue(value, &_);}
void    *NPtrValue   (NValue value) {bool _; return _NPtrValue   (value, &_);}

int      NIntValue   (NValue value) {return (int     )NInt64Value (value);}
unsigned NUIntValue  (NValue value) {return (unsigned)NUInt64Value(value);}
float    NFloatValue (NValue value) {return (float   )NDoubleValue(value);}
