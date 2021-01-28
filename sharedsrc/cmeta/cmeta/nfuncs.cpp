#include <cstdlib>
#include <cstring>
#include "ncastable.h"
#include "nstructs.h"

//the maximum number of arguments that can be supported.
#define MAX_ARG_NUM 4

struct _NFuncInfo {
    const char *name;
    void       *address;
    const char *retName;
    bool        retRetained;
    int         argCount;
    const char *argNames[MAX_ARG_NUM];

    //delayed assignment possibly.
    NType retType;
    NType argTypes[MAX_ARG_NUM];
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
    if (info->retType == 0) {

        for (int n = 0; n < info->argCount; ++n) {
            if (info->argTypes[n] == 0) {
                info->argTypes[n] = NFindStruct(info->argNames[n]);
            }
        }

        info->retType = NFindStruct(info->retName);
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
    template<class... A> static __NWord F(void *func, A... a) {
        R ret[sizeof(__NWord)] = {0};
        *ret = ((R (*)(A...))func)(a...);
        return *(__NWord *)ret;
    }
};
template<> struct _NExecutor<void> {
    template<class... A> static __NWord F(void *func, A... a) {
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
    template<class... A> static __NWord F(_NCallerData *data, A... a) {
        if (N == data->funcInfo->argCount) {
            return _NExecutor<R>::F(data->funcInfo->address, a...);
        }

        NType   d = data->funcInfo->argTypes[N];
        NType   t = data->argTypes[N];
        __NWord w = data->argWords[N];

        switch (d) {
            //only use "intptr_t", "int64_t", "float" and "double" 4 types,
            //to prevent code bloat.

            case NTypeStruct: return 0;
            case NTypePtr   : return 0;

            case NTypeBool  : return _NCaller<R, N + 1>::F(data, a..., (intptr_t)_NV<bool    >(t, w));
            case NTypeInt8  : return _NCaller<R, N + 1>::F(data, a..., (intptr_t)_NV<int8_t  >(t, w));
            case NTypeInt16 : return _NCaller<R, N + 1>::F(data, a..., (intptr_t)_NV<int16_t >(t, w));
            case NTypeInt32 : return _NCaller<R, N + 1>::F(data, a..., (intptr_t)_NV<int32_t >(t, w));
            case NTypeInt64 : return _NCaller<R, N + 1>::F(data, a..., (int64_t )_NV<int64_t >(t, w));
            case NTypeUInt8 : return _NCaller<R, N + 1>::F(data, a..., (intptr_t)_NV<uint8_t >(t, w));
            case NTypeUInt16: return _NCaller<R, N + 1>::F(data, a..., (intptr_t)_NV<uint16_t>(t, w));
            case NTypeUInt32: return _NCaller<R, N + 1>::F(data, a..., (intptr_t)_NV<uint32_t>(t, w));
            case NTypeUInt64: return _NCaller<R, N + 1>::F(data, a..., (int64_t )_NV<uint64_t>(t, w));
            case NTypeFloat : return _NCaller<R, N + 1>::F(data, a..., (float   )_NV<float   >(t, w));
            case NTypeDouble: return _NCaller<R, N + 1>::F(data, a..., (double  )_NV<double  >(t, w));
            
            default/* ptr */: return _NCaller<R, N + 1>::F(data, a..., (intptr_t)_NV<intptr_t>(t, w));
        }
    }
};
template<class R> struct _NCaller<R, MAX_ARG_NUM> {
    template<class... A> static __NWord F(_NCallerData *data, A... a) {
        return _NExecutor<R>::F(data->funcInfo->address, a...);
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
        if (!NCastable(srcType, dstType)) {
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

        case NTypeVoid  : return _NCaller<void    , 0>::F(&data);
        case NTypeBool  : return _NCaller<intptr_t, 0>::F(&data);
        case NTypeInt8  : return _NCaller<intptr_t, 0>::F(&data);
        case NTypeInt16 : return _NCaller<intptr_t, 0>::F(&data);
        case NTypeInt32 : return _NCaller<intptr_t, 0>::F(&data);
        case NTypeInt64 : return _NCaller<int64_t , 0>::F(&data);
        case NTypeUInt8 : return _NCaller<intptr_t, 0>::F(&data);
        case NTypeUInt16: return _NCaller<intptr_t, 0>::F(&data);
        case NTypeUInt32: return _NCaller<intptr_t, 0>::F(&data);
        case NTypeUInt64: return _NCaller<int64_t , 0>::F(&data);
        case NTypeFloat : return _NCaller<float   , 0>::F(&data);
        case NTypeDouble: return _NCaller<double  , 0>::F(&data);

        default/* ptr */: return _NCaller<intptr_t, 0>::F(&data);
    }
}

template<class T> struct _NTrait      {static const NType TYPE = NTypeStruct;};
template<class T> struct _NTrait<T *> {static const NType TYPE = NTypePtr   ;};

template<> struct _NTrait<void    > {static const NType TYPE = NTypeVoid  ;};
template<> struct _NTrait<bool    > {static const NType TYPE = NTypeBool  ;};
template<> struct _NTrait<int8_t  > {static const NType TYPE = NTypeInt8  ;};
template<> struct _NTrait<int16_t > {static const NType TYPE = NTypeInt16 ;};
template<> struct _NTrait<int32_t > {static const NType TYPE = NTypeInt32 ;};
template<> struct _NTrait<int64_t > {static const NType TYPE = NTypeInt64 ;};
template<> struct _NTrait<uint8_t > {static const NType TYPE = NTypeUInt8 ;};
template<> struct _NTrait<uint16_t> {static const NType TYPE = NTypeUInt16;};
template<> struct _NTrait<uint32_t> {static const NType TYPE = NTypeUInt32;};
template<> struct _NTrait<uint64_t> {static const NType TYPE = NTypeUInt64;};
template<> struct _NTrait<float   > {static const NType TYPE = NTypeFloat ;};
template<> struct _NTrait<double  > {static const NType TYPE = NTypeDouble;};

static bool _NCheckRetRetained(NType retType, const char *name) {
    if (retType != NTypePtr) {
        return false;
    }

    if (strstr(name, "Create")) {return true;}
    if (strstr(name, "Copy"  )) {return true;}
    if (strstr(name, "Retain")) {return true;}

    return false;
}

struct _NFuncAdder {

    template<class... A> _NFuncAdder(const char *name, void (*func)(A...)) {
        AddFunc(name, NTypeVoid, func);
    }
    
    template<class R, class... A> _NFuncAdder(const char *name, R (*func)(A...)) {
        AddFunc(name, _NTrait<R>::TYPE, (void (*)(A...))func);
    }
    
    template<class... A> void AddFunc(const char *name, NType retType, void (*func)(A...)) {
        
        bool error = false;
        int  count = CheckArgs(func, &error);
        
        if (error) {
            //some argument types are unsupported.
            return;
        }
        if (count > MAX_ARG_NUM) {
            //too many arguments.
            return;
        }
        if (!IsSupportedType(retType)) {
            //return type is unsupported.
            return;
        }
        
        _NFuncInfo info = {0};
        {
            info.name        = name;
            info.address     = (void *)func;
            info.retType     = retType;
            info.retRetained = _NCheckRetRetained(retType, name);

            AddArgTypes(&info, func);
        }
        _NAddFunc(&info);
    }
    
    template<class A, class... B> int CheckArgs(void (*)(A, B...), bool *error) {
        NType type = _NTrait<A>::TYPE;
        if (!IsSupportedType(type)) {
            *error = true;
        }
        
        return 1 + CheckArgs((void (*)(B...))NULL, error);
    }
    
    int CheckArgs(void (*)(), bool *) {
        return 0;
    }
    
    template<class A, class... B> void AddArgTypes(_NFuncInfo *info, void (*)(A, B...)) {
        int pos = (info->argCount)++;

        info->argTypes[pos] = _NTrait<A>::TYPE;
        AddArgTypes(info, (void (*)(B...))NULL);
    }
    
    void AddArgTypes(_NFuncInfo *, void (*)()) {
    }
    
    bool IsSupportedType(NType type) {
        //struct type is unsupported currently.
        return type != NTypeStruct;
    }
};

#undef  nfunc
#define nfunc(r, n, p) __nfunc(r, n, p); static _NFuncAdder __adder_##n(#n, n)
#include "NEXPORT.h"
