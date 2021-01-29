#include <cstdlib>
#include <cstring>
#include "nstructs.h"
#include "ntypecheck.h"

//the maximum number of arguments that can be supported.
#define MAX_ARG_NUM 4

struct _NFuncInfo {
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

static _NFuncInfo *sList = NULL;
static int sEnd = 0;
static int sConfine = 0;

static _NFuncInfo *_NReallocFuncInfo(_NFuncInfo *list, int count) {
    auto size = sizeof(_NFuncInfo) * count;
    return (_NFuncInfo *)realloc(list, size);
}

static void _NAddFunc(_NFuncInfo *info) {
    if (sEnd == sConfine) {
        sConfine += EACH_ALLOC_NUM;
        sList = _NReallocFuncInfo(sList, sConfine);

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
        memmove(dst, src, num * sizeof(_NFuncInfo));
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

//determine whether the Index is valid.
static bool _NI(int fIndex) {
    return LIST_BEGIN <= fIndex && fIndex < sEnd;
}

//get complete Function information.
static _NFuncInfo *_NF(int fIndex) {
    _NFuncInfo *info = sList + fIndex;

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

nclink const char *NFuncName       (int i) {return _NI(i) ? _NF(i)->name        : NULL ;}
nclink void       *NFuncAddress    (int i) {return _NI(i) ? _NF(i)->address     : NULL ;}
nclink int         NFuncRetType    (int i) {return _NI(i) ? _NF(i)->retType     : 0    ;}
nclink bool        NFuncRetRetained(int i) {return _NI(i) ? _NF(i)->retRetained : false;}
nclink int         NFuncArgCount   (int i) {return _NI(i) ? _NF(i)->argCount    : 0    ;}

nclink NType NFuncArgType(int fIndex, int aIndex) {
    if (_NI(fIndex)) {
        _NFuncInfo *info = _NF(fIndex);
        if (0 <= aIndex && aIndex < info->argCount) {
            return info->argTypes[aIndex];
        }
    }
    return 0;
}

//the memory layout of "__NWord" is same with "NWord" 's.
#define __NWord int64_t

template<class R> struct _NExecutor {
    template<class... A> static __NWord Exec(void *func, A... a) {
        R ret[sizeof(__NWord)] = {0};
        *ret = ((R (*)(A...))func)(a...);
        return *(__NWord *)ret;
    }
};
template<> struct _NExecutor<void> {
    template<class... A> static __NWord Exec(void *func, A... a) {
        ((void (*)(A...))func)(a...);
        return 0;
    }
};

//safe Value cast.
template<class T> T _NV(NType srcType, __NWord word) {
    switch (srcType) {
        case NTypeStruct: return (T) 0;
        case NTypePtr   : return (T) 0;

        case NTypeBool  : return (T) *(bool     *)&word;
        case NTypeInt8  : return (T) *(int8_t   *)&word;
        case NTypeInt16 : return (T) *(int16_t  *)&word;
        case NTypeInt32 : return (T) *(int32_t  *)&word;
        case NTypeInt64 : return (T) *(int64_t  *)&word;
        case NTypeUInt8 : return (T) *(uint8_t  *)&word;
        case NTypeUInt16: return (T) *(uint16_t *)&word;
        case NTypeUInt32: return (T) *(uint32_t *)&word;
        case NTypeUInt64: return (T) *(uint64_t *)&word;
        case NTypeFloat : return (T) *(float    *)&word;
        case NTypeDouble: return (T) *(double   *)&word;

        default/* ptr */: return (T) *(intptr_t *)&word;
    }
}

struct _NCallerData {
    _NFuncInfo *funcInfo;
    NType      *argTypes;
    __NWord    *argWords;
};

template<class R, int N> struct _NCaller {
    template<class... A> static __NWord C(_NCallerData *data, A... a) {
        if (N == data->funcInfo->argCount) {
            return _NExecutor<R>::Exec(data->funcInfo->address, a...);
        }

        NType   d = data->funcInfo->argTypes[N];
        NType   t = data->argTypes[N];
        __NWord w = data->argWords[N];

        switch (d) {
            //only use "intptr_t", "int64_t", "float" and "double" 4 types,
            //to prevent code bloat.

            case NTypeStruct: return 0;
            case NTypePtr   : return 0;

            case NTypeBool  : return _NCaller<R, N + 1>::C(data, a..., (intptr_t)_NV<bool    >(t, w));
            case NTypeInt8  : return _NCaller<R, N + 1>::C(data, a..., (intptr_t)_NV<int8_t  >(t, w));
            case NTypeInt16 : return _NCaller<R, N + 1>::C(data, a..., (intptr_t)_NV<int16_t >(t, w));
            case NTypeInt32 : return _NCaller<R, N + 1>::C(data, a..., (intptr_t)_NV<int32_t >(t, w));
            case NTypeInt64 : return _NCaller<R, N + 1>::C(data, a..., (int64_t )_NV<int64_t >(t, w));
            case NTypeUInt8 : return _NCaller<R, N + 1>::C(data, a..., (intptr_t)_NV<uint8_t >(t, w));
            case NTypeUInt16: return _NCaller<R, N + 1>::C(data, a..., (intptr_t)_NV<uint16_t>(t, w));
            case NTypeUInt32: return _NCaller<R, N + 1>::C(data, a..., (intptr_t)_NV<uint32_t>(t, w));
            case NTypeUInt64: return _NCaller<R, N + 1>::C(data, a..., (int64_t )_NV<uint64_t>(t, w));
            case NTypeFloat : return _NCaller<R, N + 1>::C(data, a..., (float   )_NV<float   >(t, w));
            case NTypeDouble: return _NCaller<R, N + 1>::C(data, a..., (double  )_NV<double  >(t, w));
            
            default/* ptr */: return _NCaller<R, N + 1>::C(data, a..., (intptr_t)_NV<intptr_t>(t, w));
        }
    }
};
template<class R> struct _NCaller<R, MAX_ARG_NUM> {
    template<class... A> static __NWord C(_NCallerData *data, A... a) {
        return _NExecutor<R>::Exec(data->funcInfo->address, a...);
    }
};

nclink __NWord NCallFunc(int fIndex, int argc, NType *types, __NWord *words) {
    //is it a valid index.
    if (!_NI(fIndex)) {
        return 0;
    }

    //are there enough arguments.
    _NFuncInfo *info = _NF(fIndex);
    if (argc < info->argCount) {
        return 0;
    }

    //are the argument types correct.
    for (int n = 0; n < info->argCount; ++n) {
        NType srcType = types[n];
        NType dstType = info->argTypes[n];
        if (!NSafeCastable(srcType, dstType)) {
            return 0;
        }
    }

    _NCallerData data = {0};
    data.funcInfo = info ;
    data.argTypes = types;
    data.argWords = words;

    switch (info->retType) {
        //only use "void", "intptr_t", "int64_t", "float" and "double" 5 types,
        //to prevent code bloat.

        case NTypeStruct: return 0;
        case NTypePtr   : return 0;

        case NTypeVoid  : return _NCaller<void    , 0>::C(&data);
        case NTypeBool  : return _NCaller<intptr_t, 0>::C(&data);
        case NTypeInt8  : return _NCaller<intptr_t, 0>::C(&data);
        case NTypeInt16 : return _NCaller<intptr_t, 0>::C(&data);
        case NTypeInt32 : return _NCaller<intptr_t, 0>::C(&data);
        case NTypeInt64 : return _NCaller<int64_t , 0>::C(&data);
        case NTypeUInt8 : return _NCaller<intptr_t, 0>::C(&data);
        case NTypeUInt16: return _NCaller<intptr_t, 0>::C(&data);
        case NTypeUInt32: return _NCaller<intptr_t, 0>::C(&data);
        case NTypeUInt64: return _NCaller<int64_t , 0>::C(&data);
        case NTypeFloat : return _NCaller<float   , 0>::C(&data);
        case NTypeDouble: return _NCaller<double  , 0>::C(&data);

        default/* ptr */: return _NCaller<intptr_t, 0>::C(&data);
    }
}

template<class T> struct _NTrait {
    static constexpr const char *const NAME = "Struct";
    static const NType TYPE = NTypeStruct;
};
template<class T> struct _NTrait<T *> {
    static constexpr const char *const NAME = "Ptr";
    static const NType TYPE = NTypePtr;
};
template<class T> struct _NTrait<T **> {
    static constexpr const char *const NAME = "Ptr";
    static const NType TYPE = NTypePtr;
};

#define SPECIAL_TRAIT(S, N, T)                              \
/**/    template<> struct _NTrait<S> {                      \
/**/        static constexpr const char *const NAME = N;    \
/**/        static const NType TYPE = T;                    \
/**/    }

SPECIAL_TRAIT(void    , "void"  , NTypeVoid  );
SPECIAL_TRAIT(bool    , "bool"  , NTypeBool  );
SPECIAL_TRAIT(char    , "char8" , NTypeChar8 );
SPECIAL_TRAIT(char16_t, "char16", NTypeChar16);
SPECIAL_TRAIT(char32_t, "char32", NTypeChar32);
SPECIAL_TRAIT(int8_t  , "int8"  , NTypeInt8  );
SPECIAL_TRAIT(int16_t , "int16" , NTypeInt16 );
SPECIAL_TRAIT(int32_t , "int32" , NTypeInt32 );
SPECIAL_TRAIT(int64_t , "int64" , NTypeInt64 );
SPECIAL_TRAIT(uint8_t , "uint8" , NTypeUInt8 );
SPECIAL_TRAIT(uint16_t, "uint16", NTypeUInt16);
SPECIAL_TRAIT(uint32_t, "uint32", NTypeUInt32);
SPECIAL_TRAIT(uint64_t, "uint64", NTypeUInt64);
SPECIAL_TRAIT(float   , "float" , NTypeFloat );
SPECIAL_TRAIT(double  , "double", NTypeDouble);

SPECIAL_TRAIT(void     *, "voidptr"  , NTypeVoidPtr  );
SPECIAL_TRAIT(bool     *, "boolptr"  , NTypeBoolPtr  );
SPECIAL_TRAIT(char     *, "char8ptr" , NTypeChar8Ptr );
SPECIAL_TRAIT(char16_t *, "char16ptr", NTypeChar16Ptr);
SPECIAL_TRAIT(char32_t *, "char32ptr", NTypeChar32Ptr);
SPECIAL_TRAIT(int8_t   *, "int8ptr"  , NTypeInt8Ptr  );
SPECIAL_TRAIT(int16_t  *, "int16ptr" , NTypeInt16Ptr );
SPECIAL_TRAIT(int32_t  *, "int32ptr" , NTypeInt32Ptr );
SPECIAL_TRAIT(int64_t  *, "int64ptr" , NTypeInt64Ptr );
SPECIAL_TRAIT(uint8_t  *, "uint8ptr" , NTypeUInt8Ptr );
SPECIAL_TRAIT(uint16_t *, "uint16ptr", NTypeUInt16Ptr);
SPECIAL_TRAIT(uint32_t *, "uint32ptr", NTypeUInt32Ptr);
SPECIAL_TRAIT(uint64_t *, "uint64ptr", NTypeUInt64Ptr);
SPECIAL_TRAIT(float    *, "floatptr" , NTypeFloatPtr );
SPECIAL_TRAIT(double   *, "doubleptr", NTypeDoublePtr);

#undef  nstruct
#undef  nclass
#define nstruct(n,    ...) __nstruct(n   , __VA_ARGS__); SPECIAL_TRAIT(n *, #n, 0)
#define nclass( n, s, ...) __nclass (n, s, __VA_ARGS__); SPECIAL_TRAIT(n *, #n, 0)

static bool _NCheckRetRetained(NType retType, const char *funcName) {
    if (retType >= NTypeCustomPtr) {
        if (strstr(funcName, "Create")) {return true;}
        if (strstr(funcName, "Copy"  )) {return true;}
        if (strstr(funcName, "Retain")) {return true;}
    }
    return false;
}

static void _NResetInfoComplete(_NFuncInfo *info) {
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

struct _NFuncAdder {

    template<class R, class... A> _NFuncAdder(const char *name, R (*func)(A...)) {
        AddFunc(name, func);
    }

    template<class R> bool CheckRet() {
        NType type = _NTrait<R>::TYPE;

        //the return type can't be a struct or unknown ptr.
        if (type == NTypeStruct) {return false;}
        if (type == NTypePtr   ) {return false;}

        return true;
    }

    template<class R, class A, class... B> bool CheckArgs(R (*)(A, B...), int *count) {
        NType type = _NTrait<A>::TYPE;

        if (type == NTypeStruct) {return false;} //the type can't be a struct.
        if (type == NTypePtr   ) {return false;} //the type can't be a unknown ptr.
        if (type == NTypeVoid  ) {return false;} //this is an exception.

        *count += 1;
        return CheckArgs((R (*)(B...))NULL, count);
    }
    template<class R> bool CheckArgs(R (*)(), int *) {
        return true;
    }

    template<class R, class A, class... B> void AddArgs(_NFuncInfo *info, R (*)(A, B...)) {
        int index = (info->argCount)++;

        info->argNames[index] = _NTrait<A>::NAME;
        info->argTypes[index] = _NTrait<A>::TYPE;

        AddArgs(info, (void (*)(B...))NULL);
    }
    template<class R> void AddArgs(_NFuncInfo *, R (*)()) {
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
        
        _NFuncInfo info = {0};
        {
            info.name        = name;
            info.address     = (void *)func;
            info.retName     = _NTrait<R>::NAME;
            info.retType     = _NTrait<R>::TYPE;
            info.retRetained = _NCheckRetRetained(_NTrait<R>::TYPE, name);

            AddArgs(&info, func);
            _NResetInfoComplete(&info);
        }
        _NAddFunc(&info);
    }
};

#define __add_func(n) static _NFuncAdder __adder_##n(#n, n)

#undef  nfunc
#define nfunc(r, n, p) __nfunc(r, n, p); __add_func(n)

#include "NEXPORT.h"
