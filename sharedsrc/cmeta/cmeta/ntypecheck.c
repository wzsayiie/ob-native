#include "ntypecheck.h"

static bool _NUnknown (NType t) {return t <= NTypePtr ;}
static bool _NIsVoid  (NType t) {return t == NTypeVoid;}
static bool _NIsBool  (NType t) {return t == NTypeBool;}
static bool _NIsNumber(NType t) {return t >= NTypeChar8 && t <= NTypeDouble;}
static bool _NIsPtr   (NType t) {return t >= NTypeVoidPtr;}

bool NSafeCastable(NType srcType, NType dstType) {
    if (_NUnknown(srcType) || _NUnknown(dstType)) {
        //unknown types can't be casted.
        return false;
    }

    if (_NIsVoid(dstType)) {
        //any type can be casted to void.
        return true;
    }

    if (_NIsBool(dstType)) {
        //any type can be casted to bool.
        return true;
    }

    if (_NIsNumber(dstType)) {
        if (_NIsVoid(srcType)) {
            return true;
        }
        if (_NIsBool(srcType)) {
            return true;
        }
        if (_NIsNumber(srcType)) {
            //numeric types always can be casted to each other,
            //which is more convenient for language bridging.
            return true;
        }
        if (_NIsPtr(srcType)) {
            return false;
        }
        return false;
    }

    if (_NIsPtr(dstType)) {
        //ptr can't be casted currently.
        return srcType == dstType;
    }

    return false;
}
