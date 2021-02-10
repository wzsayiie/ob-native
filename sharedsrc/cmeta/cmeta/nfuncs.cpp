#include <cstdlib>
#include <cstring>
#include "nstructs.h"
#include "ntypecheck.h"

//from "nio.h".
nclink void __NError(const char *format, ...);

//the maximum number of arguments that can be supported.
static const int MAX_ARG_NUM = 4;

struct FuncInfo {
    const char *name;
    void       *address;
    const char *retName;
    NType       retType;                //delayed assignment possibly.
    bool        retRetained;
    int         argCount;
    const char *argNames[MAX_ARG_NUM];
    NType       argTypes[MAX_ARG_NUM];  //delayed assignment possibly.
    bool        complete;
};

//the number of slots allocated each time.
static const int EACH_ALLOC_NUM = 64;

//the index "0" is reserved.
static const int LIST_BEGIN = 1;

static FuncInfo *sList = NULL;
static int sEnd = 0;
static int sConfine = 0;

static FuncInfo *ReallocFuncInfo(FuncInfo *list, int count) {
    auto size = sizeof(FuncInfo) * count;
    return (FuncInfo *)realloc(list, size);
}

static void AddFuncInfo(FuncInfo *info) {
    if (sEnd == sConfine) {
        sConfine += EACH_ALLOC_NUM;
        sList = ReallocFuncInfo(sList, sConfine);

        if (sEnd == 0) {
            sEnd = LIST_BEGIN;
        }
    }

    //insertion sort followed.
    int begin = LIST_BEGIN;
    int end   = sEnd;
    int index = (begin + end) / 2;
    while (begin < end) {
        int result = strcmp(info->name, sList[index].name);
        if (result == 0) {
            //the item already exists. this is a exception.
            return;
        }

        if (result > 0) {
            begin = index + 1;
        } else {
            end = index;
        }

        index = (begin + end) / 2;
    }

    //insert new item.
    if (index < sEnd) {
        void *dst = sList + index + 1;
        void *src = sList + index;
        int   num = sEnd  - index;
        memmove(dst, src, num * sizeof(FuncInfo));
    }
    sList[index] = *info;
    sEnd += 1;
}

nclink int NFuncsBegin() {return LIST_BEGIN;}
nclink int NFuncsEnd  () {return sEnd;}

nclink int NFindFunc(const char *name) {
    if (!name || *name == '\0') {
        return 0;
    }

    //binary search here.
    int begin = LIST_BEGIN;
    int end   = sEnd;

    while (begin < end) {
        int center = (begin + end) / 2;
        int result = strcmp(name, sList[center].name);

        if (result > 0) {
            begin = center + 1;
        } else if (result < 0) {
            end = center;
        } else {
            return center;
        }
    }
    return 0;
}

//determine whether the index is valid.
static bool Idx(int fIndex) {
    return LIST_BEGIN <= fIndex && fIndex < sEnd;
}

//get complete function information.
static FuncInfo *Inf(int fIndex) {
    FuncInfo *info = sList + fIndex;

    //mutex is not necessary here, cause the value obtained by each thread is the same.
    if (!info->complete) {
        if (info->retType == 0) {
            info->retType = NFindStruct(info->retName);
        }

        for (int n = 0; n < info->argCount; ++n) {
            if (info->argTypes[n] == 0) {
                info->argTypes[n] = NFindStruct(info->argNames[n]);
            }
        }

        info->complete = true;
    }

    return info;
}

nclink const char *NFuncName       (int i) {return Idx(i) ? Inf(i)->name        : NULL ;}
nclink void       *NFuncAddress    (int i) {return Idx(i) ? Inf(i)->address     : NULL ;}
nclink int         NFuncRetType    (int i) {return Idx(i) ? Inf(i)->retType     : 0    ;}
nclink bool        NFuncRetRetained(int i) {return Idx(i) ? Inf(i)->retRetained : false;}
nclink int         NFuncArgCount   (int i) {return Idx(i) ? Inf(i)->argCount    : 0    ;}

nclink NType NFuncArgType(int fIndex, int aIndex) {
    if (Idx(fIndex)) {
        FuncInfo *info = Inf(fIndex);
        if (0 <= aIndex && aIndex < info->argCount) {
            return info->argTypes[aIndex];
        }
    }
    return 0;
}

//the memory layout of "_NWord" is same with "NWord" 's.
#define _NWord int64_t

template<class R> struct Executor {
    template<class... A> static _NWord Exec(void *func, A... a) {
        R ret[sizeof(_NWord)] = {0};
        *ret = ((R (*)(A...))func)(a...);
        return *(_NWord *)ret;
    }
};
template<> struct Executor<void> {
    template<class... A> static _NWord Exec(void *func, A... a) {
        ((void (*)(A...))func)(a...);
        return 0;
    }
};

//safe value cast.
template<class T> T Val(NType srcType, _NWord word) {
    switch (srcType) {
        case NTYPE_STRUCT: return (T) 0;
        case NTYPE_PTR   : return (T) 0;

        case NTYPE_BOOL  : return (T) *(bool     *)&word;
        case NTYPE_INT8  : return (T) *(int8_t   *)&word;
        case NTYPE_INT16 : return (T) *(int16_t  *)&word;
        case NTYPE_INT32 : return (T) *(int32_t  *)&word;
        case NTYPE_INT64 : return (T) *(int64_t  *)&word;
        case NTYPE_UINT8 : return (T) *(uint8_t  *)&word;
        case NTYPE_UINT16: return (T) *(uint16_t *)&word;
        case NTYPE_UINT32: return (T) *(uint32_t *)&word;
        case NTYPE_UINT64: return (T) *(uint64_t *)&word;
        case NTYPE_FLOAT : return (T) *(float    *)&word;
        case NTYPE_DOUBLE: return (T) *(double   *)&word;

        default /* ptr */: return (T) *(intptr_t *)&word;
    }
}

struct CallerData {
    FuncInfo *funcInfo;
    NType    *argTypes;
    _NWord   *argWords;
};

template<class R, int N> struct Caller {
    template<class... A> static _NWord C(CallerData *data, A... a) {
        if (N == data->funcInfo->argCount) {
            return Executor<R>::Exec(data->funcInfo->address, a...);
        }

        NType  d = data->funcInfo->argTypes[N];
        NType  t = data->argTypes[N];
        _NWord w = data->argWords[N];

        switch (d) {
            //only use "intptr_t", "int64_t", "float" and "double" 4 types,
            //to prevent code bloat.

            case NTYPE_STRUCT: return 0;
            case NTYPE_PTR   : return 0;

            case NTYPE_BOOL  : return Caller<R, N + 1>::C(data, a..., (intptr_t)Val<bool    >(t, w));
            case NTYPE_INT8  : return Caller<R, N + 1>::C(data, a..., (intptr_t)Val<int8_t  >(t, w));
            case NTYPE_INT16 : return Caller<R, N + 1>::C(data, a..., (intptr_t)Val<int16_t >(t, w));
            case NTYPE_INT32 : return Caller<R, N + 1>::C(data, a..., (intptr_t)Val<int32_t >(t, w));
            case NTYPE_INT64 : return Caller<R, N + 1>::C(data, a..., (int64_t )Val<int64_t >(t, w));
            case NTYPE_UINT8 : return Caller<R, N + 1>::C(data, a..., (intptr_t)Val<uint8_t >(t, w));
            case NTYPE_UINT16: return Caller<R, N + 1>::C(data, a..., (intptr_t)Val<uint16_t>(t, w));
            case NTYPE_UINT32: return Caller<R, N + 1>::C(data, a..., (intptr_t)Val<uint32_t>(t, w));
            case NTYPE_UINT64: return Caller<R, N + 1>::C(data, a..., (int64_t )Val<uint64_t>(t, w));
            case NTYPE_FLOAT : return Caller<R, N + 1>::C(data, a..., (float   )Val<float   >(t, w));
            case NTYPE_DOUBLE: return Caller<R, N + 1>::C(data, a..., (double  )Val<double  >(t, w));
            
            default /* ptr */: return Caller<R, N + 1>::C(data, a..., (intptr_t)Val<intptr_t>(t, w));
        }
    }
};
template<class R> struct Caller<R, MAX_ARG_NUM> {
    template<class... A> static _NWord C(CallerData *data, A... a) {
        return Executor<R>::Exec(data->funcInfo->address, a...);
    }
};

nclink _NWord NCallFunc(int fIndex, int argc, NType *types, _NWord *words) {
    //is it a valid index.
    if (!Idx(fIndex)) {
        __NError("illegal function index %d", fIndex);
        return 0;
    }

    //are there enough arguments.
    FuncInfo *info = Inf(fIndex);
    if (argc < info->argCount) {
        __NError("only %d arguments passed, but %d required", argc, info->argCount);
        return 0;
    }

    //are the argument types correct.
    for (int n = 0; n < info->argCount; ++n) {
        NType srcType = types[n];
        NType dstType = info->argTypes[n];
        if (!NSafeCastable(srcType, dstType)) {
            __NError("argument %d can't cast from type %d to %d", n + 1, srcType, dstType);
            return 0;
        }
    }

    CallerData data = {0};
    data.funcInfo = info ;
    data.argTypes = types;
    data.argWords = words;

    switch (info->retType) {
        //only use "void", "intptr_t", "int64_t", "float" and "double" 5 types,
        //to prevent code bloat.

        case NTYPE_STRUCT: return 0;
        case NTYPE_PTR   : return 0;

        case NTYPE_VOID  : return Caller<void    , 0>::C(&data);
        case NTYPE_BOOL  : return Caller<intptr_t, 0>::C(&data);
        case NTYPE_INT8  : return Caller<intptr_t, 0>::C(&data);
        case NTYPE_INT16 : return Caller<intptr_t, 0>::C(&data);
        case NTYPE_INT32 : return Caller<intptr_t, 0>::C(&data);
        case NTYPE_INT64 : return Caller<int64_t , 0>::C(&data);
        case NTYPE_UINT8 : return Caller<intptr_t, 0>::C(&data);
        case NTYPE_UINT16: return Caller<intptr_t, 0>::C(&data);
        case NTYPE_UINT32: return Caller<intptr_t, 0>::C(&data);
        case NTYPE_UINT64: return Caller<int64_t , 0>::C(&data);
        case NTYPE_FLOAT : return Caller<float   , 0>::C(&data);
        case NTYPE_DOUBLE: return Caller<double  , 0>::C(&data);

        default /* ptr */: return Caller<intptr_t, 0>::C(&data);
    }
}

template<class T> struct Trait {
    static constexpr const char *const NAME = "struct";
    static const NType TYPE = NTYPE_STRUCT;
};
template<class T> struct Trait<T *> {
    static constexpr const char *const NAME = "ptr";
    static const NType TYPE = NTYPE_PTR;
};
template<class T> struct Trait<T **> {
    static constexpr const char *const NAME = "ptr";
    static const NType TYPE = NTYPE_PTR;
};

#define SPECIAL_TRAIT(S, N, T)                              \
/**/    template<> struct Trait<S> {                      \
/**/        static constexpr const char *const NAME = N;    \
/**/        static const NType TYPE = T;                    \
/**/    }

SPECIAL_TRAIT(void    , "void"  , NTYPE_VOID  );
SPECIAL_TRAIT(bool    , "bool"  , NTYPE_BOOL  );
SPECIAL_TRAIT(char    , "char8" , NTYPE_CHAR8 );
SPECIAL_TRAIT(char16_t, "char16", NTYPE_CHAR16);
SPECIAL_TRAIT(char32_t, "char32", NTYPE_CHAR32);
SPECIAL_TRAIT(int8_t  , "int8"  , NTYPE_INT8  );
SPECIAL_TRAIT(int16_t , "int16" , NTYPE_INT16 );
SPECIAL_TRAIT(int32_t , "int32" , NTYPE_INT32 );
SPECIAL_TRAIT(int64_t , "int64" , NTYPE_INT64 );
SPECIAL_TRAIT(uint8_t , "uint8" , NTYPE_UINT8 );
SPECIAL_TRAIT(uint16_t, "uint16", NTYPE_UINT16);
SPECIAL_TRAIT(uint32_t, "uint32", NTYPE_UINT32);
SPECIAL_TRAIT(uint64_t, "uint64", NTYPE_UINT64);
SPECIAL_TRAIT(float   , "float" , NTYPE_FLOAT );
SPECIAL_TRAIT(double  , "double", NTYPE_DOUBLE);

SPECIAL_TRAIT(void     *, "voidptr"  , NTYPE_VOID_PTR  );
SPECIAL_TRAIT(bool     *, "boolptr"  , NTYPE_BOOL_PTR  );
SPECIAL_TRAIT(char     *, "char8ptr" , NTYPE_CHAR8_PTR );
SPECIAL_TRAIT(char16_t *, "char16ptr", NTYPE_CHAR16_PTR);
SPECIAL_TRAIT(char32_t *, "char32ptr", NTYPE_CHAR32_PTR);
SPECIAL_TRAIT(int8_t   *, "int8ptr"  , NTYPE_INT8_PTR  );
SPECIAL_TRAIT(int16_t  *, "int16ptr" , NTYPE_INT16_PTR );
SPECIAL_TRAIT(int32_t  *, "int32ptr" , NTYPE_INT32_PTR );
SPECIAL_TRAIT(int64_t  *, "int64ptr" , NTYPE_INT64_PTR );
SPECIAL_TRAIT(uint8_t  *, "uint8ptr" , NTYPE_UINT8_PTR );
SPECIAL_TRAIT(uint16_t *, "uint16ptr", NTYPE_UINT16_PTR);
SPECIAL_TRAIT(uint32_t *, "uint32ptr", NTYPE_UINT32_PTR);
SPECIAL_TRAIT(uint64_t *, "uint64ptr", NTYPE_UINT64_PTR);
SPECIAL_TRAIT(float    *, "floatptr" , NTYPE_FLOAT_PTR );
SPECIAL_TRAIT(double   *, "doubleptr", NTYPE_DOUBLE_PTR);

SPECIAL_TRAIT(const void     *, "voidptr"  , NTYPE_VOID_PTR  );
SPECIAL_TRAIT(const bool     *, "boolptr"  , NTYPE_BOOL_PTR  );
SPECIAL_TRAIT(const char     *, "char8ptr" , NTYPE_CHAR8_PTR );
SPECIAL_TRAIT(const char16_t *, "char16ptr", NTYPE_CHAR16_PTR);
SPECIAL_TRAIT(const char32_t *, "char32ptr", NTYPE_CHAR32_PTR);
SPECIAL_TRAIT(const int8_t   *, "int8ptr"  , NTYPE_INT8_PTR  );
SPECIAL_TRAIT(const int16_t  *, "int16ptr" , NTYPE_INT16_PTR );
SPECIAL_TRAIT(const int32_t  *, "int32ptr" , NTYPE_INT32_PTR );
SPECIAL_TRAIT(const int64_t  *, "int64ptr" , NTYPE_INT64_PTR );
SPECIAL_TRAIT(const uint8_t  *, "uint8ptr" , NTYPE_UINT8_PTR );
SPECIAL_TRAIT(const uint16_t *, "uint16ptr", NTYPE_UINT16_PTR);
SPECIAL_TRAIT(const uint32_t *, "uint32ptr", NTYPE_UINT32_PTR);
SPECIAL_TRAIT(const uint64_t *, "uint64ptr", NTYPE_UINT64_PTR);
SPECIAL_TRAIT(const float    *, "floatptr" , NTYPE_FLOAT_PTR );
SPECIAL_TRAIT(const double   *, "doubleptr", NTYPE_DOUBLE_PTR);

#undef  nstruct
#undef  nclass
#define nstruct(n,    ...) __nstruct(n   , __VA_ARGS__); SPECIAL_TRAIT(n *, #n, 0)
#define nclass( n, s, ...) __nclass (n, s, __VA_ARGS__); SPECIAL_TRAIT(n *, #n, 0)

static bool CheckRetRetained(NType retType, const char *funcName) {
    if (retType >= NTYPE_CUSTOM_PTR) {
        if (strstr(funcName, "Create")) {return true;}
        if (strstr(funcName, "Copy"  )) {return true;}
        if (strstr(funcName, "Retain")) {return true;}
    }
    return false;
}

static void ResetInfoComplete(FuncInfo *info) {
    //check data maybe delayed in assignment.
    if (info->retType == 0) {
        info->complete = false;
        return;
    }
    for (int n = 0; n < (info->argCount); ++n) {
        if (info->argTypes[n] == 0) {
            info->complete = false;
            return;
        }
    }

    info->complete = true;
}

struct FuncAdder {

    template<class R, class... A> FuncAdder(const char *name, R (*func)(A...)) {
        AddFunc(name, func);
    }

    template<class R> bool CheckRet() {
        NType type = Trait<R>::TYPE;

        //the return type can't be a struct or unknown ptr.
        if (type == NTYPE_STRUCT) {return false;}
        if (type == NTYPE_PTR   ) {return false;}

        return true;
    }

    template<class R, class A, class... B> bool CheckArgs(R (*)(A, B...), int *count) {
        NType type = Trait<A>::TYPE;

        if (type == NTYPE_STRUCT) {return false;} //the type can't be a struct.
        if (type == NTYPE_PTR   ) {return false;} //the type can't be a unknown ptr.
        if (type == NTYPE_VOID  ) {return false;} //this is an exception.

        *count += 1;
        return CheckArgs((R (*)(B...))NULL, count);
    }
    template<class R> bool CheckArgs(R (*)(), int *) {
        return true;
    }

    template<class R, class A, class... B> void AddArgs(FuncInfo *info, R (*)(A, B...)) {
        int index = (info->argCount)++;

        info->argNames[index] = Trait<A>::NAME;
        info->argTypes[index] = Trait<A>::TYPE;

        AddArgs(info, (void (*)(B...))NULL);
    }
    template<class R> void AddArgs(FuncInfo *, R (*)()) {
    }
    
    template<class R, class... A> void AddFunc(const char *name, R (*func)(A...)) {
        bool retOkay = CheckRet<R>();
        if (!retOkay) {
            return;
        }

        int argCount = 0;
        bool argOkay = CheckArgs(func, &argCount);
        if (!argOkay) {
            return;
        }
        if (argCount > MAX_ARG_NUM) {
            return;
        }
        
        FuncInfo info = {0};
        {
            info.name        = name;
            info.address     = (void *)func;
            info.retName     = Trait<R>::NAME;
            info.retType     = Trait<R>::TYPE;
            info.retRetained = CheckRetRetained(Trait<R>::TYPE, name);

            AddArgs(&info, func);
            ResetInfoComplete(&info);
        }
        AddFuncInfo(&info);
    }
};

#define __add_func(n) static FuncAdder __adder_##n(#n, n)

#undef  nfunc
#define nfunc(r, n, p) __nfunc(r, n, p); __add_func(n)

#include "NEXPORT.h"
