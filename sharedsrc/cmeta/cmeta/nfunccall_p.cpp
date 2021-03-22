#include "nfunccall_p.h"
#include "basis.h"
#include "nfuncmeta_p.h"
#include "ntypemeta_p.h"

static NType DynamicType(NType staticType, _NWord word) {
    if (NTypeIsRef(staticType)) {
        auto object = (NObject *)word;
        return FindType(object->clsName);
    }
    return staticType;
}

static bool SafeCastable(NType srcType, NType dstType) {
    if (NTypeIsBlur(srcType) || NTypeIsBlur(dstType)) {
        //blur types can't be casted.
        return false;
    }

    if (NTypeIsVoid(dstType)) {
        //any type can be casted to void.
        return true;
    }

    if (NTypeIsBool(dstType)) {
        //any type can be casted to bool.
        return true;
    }

    if (NTypeIsNum(dstType)) {
        if (NTypeIsVoid(srcType)) {
            return true;
        }
        if (NTypeIsBool(srcType)) {
            return true;
        }
        if (NTypeIsNum(srcType)) {
            //numeric types always can be casted to each other,
            //which is more convenient for cross-language calling.
            return true;
        }
        
        return false;
    }

    if (NTypeIsPtr(dstType)) {
        return dstType == srcType;
    }

    //here dstType is object ref:
    if (!NTypeIsRef(srcType)) {
        return false;
    }
    for (; srcType; srcType = TypeSuper(srcType)) {
        if (dstType == srcType) {
            return true;
        }
    }

    return false;
}

struct StatusData {

    StatusData(int func, int argc, NType *types, _NWord *words) {

        funcIndex    = func;
        funcAddress  = FuncAddress (func);
        funcRetType  = FuncRetType (func);
        funcArgCount = FuncArgCount(func);

        for (int idx = 0; idx < funcArgCount; ++idx) {
            funcArgTypes[idx] = FuncArgType(func, idx);
        }

        pushedArgCount = argc;
        NMoveMemory(pushedArgTypes, types, nsizeof(*types) * FUNC_MAX_ARG_NUM);
        NMoveMemory(pushedArgWords, words, nsizeof(*words) * FUNC_MAX_ARG_NUM);
    }

    int   funcIndex;
    void *funcAddress;
    NType funcRetType;
    int   funcArgCount;
    NType funcArgTypes[FUNC_MAX_ARG_NUM];

    int    pushedArgCount;
    NType  pushedArgTypes[FUNC_MAX_ARG_NUM];
    _NWord pushedArgWords[FUNC_MAX_ARG_NUM];
};

static bool CheckStatus(StatusData *status) {
    //is function index valid?
    if (!status->funcAddress) {
        _NError("illegal function index %d", status->funcIndex);
        return false;
    }

    //is arguments enough?
    if ((status->pushedArgCount) < (status->funcArgCount)) {
        _NError("only %d arguments passed, but %d required", status->pushedArgCount, status->funcArgCount);
        return false;
    }

    //are argument types correct?
    for (int cnt = 0; cnt < (status->funcArgCount); ++cnt) {
        //"0" is legal for any type.
        if (status->pushedArgWords[cnt] == 0) {
            continue;
        }

        NType src = DynamicType(status->pushedArgTypes[cnt], status->pushedArgWords[cnt]);
        NType dst = status->funcArgTypes[cnt];
        if (!SafeCastable(src, dst)) {
            _NError("argument %d can't cast from type %d to %d", cnt + 1, src, dst);
            return false;
        }
    }

    return true;
}

template<class Ret>
struct Executor {

    template<class... Arg>
    static _NWord Exec(void *func, Arg... arg) {
        Ret ret[sizeof(_NWord)] = {0};
        *ret = ((Ret (*)(Arg...))func)(arg...);
        return *(_NWord *)ret;
    }
};

template<>
struct Executor<void> {

    template<class... Arg>
    static _NWord Exec(void *func, Arg... arg) {
        ((void (*)(Arg...))func)(arg...);
        return 0;
    }
};

template<class Type>
Type Cast(NType srcType, _NWord word) {

    if (srcType == NTYPE_STRUCT) return (Type) 0;
    if (srcType == NTYPE_PTR   ) return (Type) 0;
    if (srcType == NTYPE_BOOL  ) return (Type) *(bool     *)&word;
    if (srcType == NTYPE_INT8  ) return (Type) *(int8_t   *)&word;
    if (srcType == NTYPE_INT16 ) return (Type) *(int16_t  *)&word;
    if (srcType == NTYPE_INT32 ) return (Type) *(int32_t  *)&word;
    if (srcType == NTYPE_INT64 ) return (Type) *(int64_t  *)&word;
    if (srcType == NTYPE_UINT8 ) return (Type) *(uint8_t  *)&word;
    if (srcType == NTYPE_UINT16) return (Type) *(uint16_t *)&word;
    if (srcType == NTYPE_UINT32) return (Type) *(uint32_t *)&word;
    if (srcType == NTYPE_UINT64) return (Type) *(uint64_t *)&word;
    if (srcType == NTYPE_FLOAT ) return (Type) *(float    *)&word;
    if (srcType == NTYPE_DOUBLE) return (Type) *(double   *)&word;
    else /* pointer/reference */ return (Type) *(intptr_t *)&word;
}

template<class Ret, int N>
struct Caller {

    template<class... Arg>
    static _NWord Call(StatusData *status, Arg... arg) {
        if (N == status->funcArgCount) {
            return Executor<Ret>::Exec(status->funcAddress, arg...);
        }

        NType  dstType = status->funcArgTypes  [N];
        NType  srcType = status->pushedArgTypes[N];
        _NWord word    = status->pushedArgWords[N];

        //only use "intptr_t", "int64_t", "float" and "double" 4 types, to prevent code bloat.
        if (dstType == NTYPE_STRUCT) return 0;
        if (dstType == NTYPE_PTR   ) return 0;
        if (dstType == NTYPE_BOOL  ) return Caller<Ret, N + 1>::Call(status, arg..., (intptr_t)Cast<bool    >(srcType, word));
        if (dstType == NTYPE_INT8  ) return Caller<Ret, N + 1>::Call(status, arg..., (intptr_t)Cast<int8_t  >(srcType, word));
        if (dstType == NTYPE_INT16 ) return Caller<Ret, N + 1>::Call(status, arg..., (intptr_t)Cast<int16_t >(srcType, word));
        if (dstType == NTYPE_INT32 ) return Caller<Ret, N + 1>::Call(status, arg..., (intptr_t)Cast<int32_t >(srcType, word));
        if (dstType == NTYPE_INT64 ) return Caller<Ret, N + 1>::Call(status, arg..., (int64_t )Cast<int64_t >(srcType, word));
        if (dstType == NTYPE_UINT8 ) return Caller<Ret, N + 1>::Call(status, arg..., (intptr_t)Cast<uint8_t >(srcType, word));
        if (dstType == NTYPE_UINT16) return Caller<Ret, N + 1>::Call(status, arg..., (intptr_t)Cast<uint16_t>(srcType, word));
        if (dstType == NTYPE_UINT32) return Caller<Ret, N + 1>::Call(status, arg..., (intptr_t)Cast<uint32_t>(srcType, word));
        if (dstType == NTYPE_UINT64) return Caller<Ret, N + 1>::Call(status, arg..., (int64_t )Cast<uint64_t>(srcType, word));
        if (dstType == NTYPE_FLOAT ) return Caller<Ret, N + 1>::Call(status, arg..., (float   )Cast<float   >(srcType, word));
        if (dstType == NTYPE_DOUBLE) return Caller<Ret, N + 1>::Call(status, arg..., (double  )Cast<double  >(srcType, word));
        else /* pointer/reference */ return Caller<Ret, N + 1>::Call(status, arg..., (intptr_t)Cast<intptr_t>(srcType, word));
    }
};

template<class Ret>
struct Caller<Ret, FUNC_MAX_ARG_NUM> {

    template<class... Arg>
    static _NWord Call(StatusData *status, Arg... arg) {
        return Executor<Ret>::Exec(status->funcAddress, arg...);
    }
};

static nthreadlocal int    sArgCount = 0;
static nthreadlocal NType  sArgTypes[FUNC_MAX_ARG_NUM] = {0};
static nthreadlocal _NWord sArgWords[FUNC_MAX_ARG_NUM] = {0};

void FuncPrepare() {
    sArgCount = 0;
}

void FuncPushArg(NType type, _NWord word) {
    if (sArgCount == FUNC_MAX_ARG_NUM) {
        _NError("only supports up to %d arguments", FUNC_MAX_ARG_NUM);
        return;
    }

    sArgTypes[sArgCount] = type;
    sArgWords[sArgCount] = word;
    sArgCount += 1;
}

_NWord FuncCall(int funcIndex) {

    //NOTE: don't use the cached data directly, so the function can be called recursively.
    StatusData status(funcIndex, sArgCount, sArgTypes, sArgWords);
    FuncPrepare();

    bool statusOkay = CheckStatus(&status);
    if (!statusOkay) {
        return 0;
    }

    switch (status.funcRetType) {
        //only use "void", "intptr_t", "int64_t", "float" and "double" 5 types, to prevent code bloat.
        case NTYPE_STRUCT: return 0;
        case NTYPE_PTR   : return 0;
        case NTYPE_VOID  : return Caller<void    , 0>::Call(&status);
        case NTYPE_BOOL  : return Caller<intptr_t, 0>::Call(&status);
        case NTYPE_INT8  : return Caller<intptr_t, 0>::Call(&status);
        case NTYPE_INT16 : return Caller<intptr_t, 0>::Call(&status);
        case NTYPE_INT32 : return Caller<intptr_t, 0>::Call(&status);
        case NTYPE_INT64 : return Caller<int64_t , 0>::Call(&status);
        case NTYPE_UINT8 : return Caller<intptr_t, 0>::Call(&status);
        case NTYPE_UINT16: return Caller<intptr_t, 0>::Call(&status);
        case NTYPE_UINT32: return Caller<intptr_t, 0>::Call(&status);
        case NTYPE_UINT64: return Caller<int64_t , 0>::Call(&status);
        case NTYPE_FLOAT : return Caller<float   , 0>::Call(&status);
        case NTYPE_DOUBLE: return Caller<double  , 0>::Call(&status);
        default /* ptr */: return Caller<intptr_t, 0>::Call(&status);
    }
}
