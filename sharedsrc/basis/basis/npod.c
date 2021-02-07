#include "npod.h"

NValue NMakeBoolValue  (bool     v) {NValue r = {0}; r.type = NTYPE_BOOL  ; r.asBool   = v; return r;}
NValue NMakeInt64Value (int64_t  v) {NValue r = {0}; r.type = NTYPE_INT64 ; r.asInt64  = v; return r;}
NValue NMakeUInt64Value(uint64_t v) {NValue r = {0}; r.type = NTYPE_UINT64; r.asUInt64 = v; return r;}
NValue NMakeDoubleValue(double   v) {NValue r = {0}; r.type = NTYPE_DOUBLE; r.asDouble = v; return r;}
NValue NMakePtrValue   (void    *v) {NValue r = {0}; r.type = NTYPE_PTR   ; r.asPtr    = v; return r;}

NValue NMakeIntValue   (int      v) {return NMakeInt64Value (v);}
NValue NMakeUIntValue  (unsigned v) {return NMakeUInt64Value(v);}
NValue NMakeFloatValue (float    v) {return NMakeDoubleValue(v);}

static bool _NIsPtr    (NType t) {return t == NTYPE_PTR  || t >= NTYPE_VOID_PTR;}
static bool _NIsInteger(NType t) {return t >= NTYPE_BOOL && t <= NTYPE_UINT64  ;}
static bool _NIsStruct (NType t) {return t == NTYPE_STRUCT;}
static bool _NIsVoid   (NType t) {return t == NTYPE_VOID  ;}
static bool _NIsFloat  (NType t) {return t == NTYPE_FLOAT ;}
static bool _NIsDouble (NType t) {return t == NTYPE_DOUBLE;}

bool NBoolValue(NValue value) {
    return value.asInt64 != 0;
}

int64_t NInt64Value(NValue value) {
    if (_NIsStruct(value.type)) {
        return 0;
    }
    if (_NIsPtr(value.type)) {
        //it's allowed to cast a ptr to an int.
        return value.asInt64;
    }
    if (_NIsVoid(value.type)) {
        return 0;
    }
    if (_NIsInteger(value.type)) {
        return value.asInt64;
    }
    if (_NIsFloat(value.type)) {
        return (int64_t)value.asDouble;
    }
    if (_NIsDouble(value.type)) {
        return (int64_t)value.asDouble;
    }
    return 0;
}

uint64_t NUInt64Value(NValue value) {
    if (_NIsStruct(value.type)) {
        return 0;
    }
    if (_NIsPtr(value.type)) {
        //it's allowed to cast a ptr to an int.
        return value.asUInt64;
    }
    if (_NIsVoid(value.type)) {
        return 0;
    }
    if (_NIsInteger(value.type)) {
        return value.asUInt64;
    }
    if (_NIsFloat(value.type)) {
        return (uint64_t)value.asDouble;
    }
    if (_NIsDouble(value.type)) {
        return (uint64_t)value.asDouble;
    }
    return 0;
}

double NDoubleValue(NValue value) {
    if (_NIsFloat(value.type) || _NIsDouble(value.type)) {
        return value.asDouble;
    }
    return 0;
}

void *NPtrValue(NValue value) {
    if (_NIsPtr(value.type)) {
        return value.asPtr;
    }
    return NULL;
}

int      NIntValue  (NValue v) {return (int     )NInt64Value (v);}
unsigned NUIntValue (NValue v) {return (unsigned)NUInt64Value(v);}
float    NFloatValue(NValue v) {return (float   )NDoubleValue(v);}
