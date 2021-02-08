#include "ntypecheck.h"

static bool Unknown (NType t) {return t <= NTYPE_PTR ;}
static bool IsVoid  (NType t) {return t == NTYPE_VOID;}
static bool IsBool  (NType t) {return t == NTYPE_BOOL;}
static bool IsNumber(NType t) {return t >= NTYPE_CHAR8 && t <= NTYPE_DOUBLE;}
static bool IsPtr   (NType t) {return t >= NTYPE_VOID_PTR;}

bool NSafeCastable(NType srcType, NType dstType) {
    if (Unknown(srcType) || Unknown(dstType)) {
        //unknown types can't be casted.
        return false;
    }

    if (IsVoid(dstType)) {
        //any type can be casted to void.
        return true;
    }

    if (IsBool(dstType)) {
        //any type can be casted to bool.
        return true;
    }

    if (IsNumber(dstType)) {
        if (IsVoid(srcType)) {
            return true;
        }
        if (IsBool(srcType)) {
            return true;
        }
        if (IsNumber(srcType)) {
            //numeric types always can be casted to each other,
            //which is more convenient for language bridging.
            return true;
        }
        if (IsPtr(srcType)) {
            return false;
        }
        return false;
    }

    if (IsPtr(dstType)) {
        //ptr can't be casted currently.
        return srcType == dstType;
    }

    return false;
}
