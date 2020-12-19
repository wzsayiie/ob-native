#include <cstring>
#include "ntypes.h"

#ifndef NFUNC_MAX_ARG_NUM
#define NFUNC_MAX_ARG_NUM 6
#endif

struct _NFuncInfo {
    const char *name;

    void *address;
    NType retType;
    bool  retRetained;
    int   argCount;
    NType argTypes[NFUNC_MAX_ARG_NUM];
};

static const int  LIST_BEGIN      = 1;
static const int  LIST_END        = 256;
static _NFuncInfo gList[LIST_END] = {0};
static int        gListEnd        = 0;

static void _NAddFunc(_NFuncInfo *info) {
    if (gListEnd == LIST_END) {
        return;
    }

    //insertion sort here.
    int pos = LIST_BEGIN;
    while (pos < gListEnd) {
        if (strcmp(gList[pos].name, info->name) > 0) {
            break;
        }
        pos++;
    }

    if (pos < gListEnd) {
        void *dst = gList + pos + 1;
        void *src = gList + pos;
        int   num = gListEnd - pos;
        memmove(dst, src, num * sizeof(_NFuncInfo));
    }
    gList[pos] = *info;
    gListEnd += 1;
}

nclink int NFuncsBegin() {return LIST_BEGIN;}
nclink int NFuncsEnd  () {return gListEnd  ;}

nclink int NFindFunc(const char *name) {
    if (name == NULL || *name == '\0') {
        return 0;
    }

    //binary search here.
    int begin = LIST_BEGIN;
    int end   = gListEnd  ;

    while (begin < end) {
        int center = (begin + end) / 2;
        int result = strcmp(name, gList[center].name);

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

static _NFuncInfo *_NFuncGetInfo(int fPos) {
    if (LIST_BEGIN <= fPos && fPos < gListEnd) {
        return gList + fPos;
    } else {
        return gList;
    }
}

nclink const char *NFuncName       (int i) {return _NFuncGetInfo(i)->name       ;}
nclink void       *NFuncAddress    (int i) {return _NFuncGetInfo(i)->address    ;}
nclink int         NFuncRetType    (int i) {return _NFuncGetInfo(i)->retType    ;}
nclink bool        NFuncRetRetained(int i) {return _NFuncGetInfo(i)->retRetained;}
nclink int         NFuncArgCount   (int i) {return _NFuncGetInfo(i)->argCount   ;}

nclink NType NFuncArgType(int fPos, int aPos) {
    _NFuncInfo *info = _NFuncGetInfo(fPos);
    if (0 <= aPos && aPos < info->argCount) {
        return info->argTypes[aPos];
    }
    return 0;
}

nclink int64_t NCallFunc(int fPos, int argc, NValue *argv) {
    return 0;
}

static bool _NFuncRetRetained(NType retType, const char *name) {
    if (retType != NTypePtr) {
        return false;
    }

    if (strstr(name, "Create")) {return true;}
    if (strstr(name, "Copy"  )) {return true;}
    if (strstr(name, "Retain")) {return true;}

    return false;
}

template<class T> struct _NTypePicker      {static const NType Type = NTypeStruct;};
template<class T> struct _NTypePicker<T *> {static const NType Type = NTypePtr   ;};

template<> struct _NTypePicker<void    > {static const NType Type = NTypeVoid  ;};
template<> struct _NTypePicker<bool    > {static const NType Type = NTypeBool  ;};
template<> struct _NTypePicker<int8_t  > {static const NType Type = NTypeInt8  ;};
template<> struct _NTypePicker<int16_t > {static const NType Type = NTypeInt16 ;};
template<> struct _NTypePicker<int32_t > {static const NType Type = NTypeInt32 ;};
template<> struct _NTypePicker<int64_t > {static const NType Type = NTypeInt64 ;};
template<> struct _NTypePicker<uint8_t > {static const NType Type = NTypeUInt8 ;};
template<> struct _NTypePicker<uint16_t> {static const NType Type = NTypeUInt16;};
template<> struct _NTypePicker<uint32_t> {static const NType Type = NTypeUInt32;};
template<> struct _NTypePicker<uint64_t> {static const NType Type = NTypeUInt64;};
template<> struct _NTypePicker<float   > {static const NType Type = NTypeFloat ;};
template<> struct _NTypePicker<double  > {static const NType Type = NTypeDouble;};

struct _NFuncAdder {

    template<class... A> _NFuncAdder(const char *name, void (*func)(A...)) {
        AddFunc(name, NTypeVoid, func);
    }
    
    template<class R, class... A> _NFuncAdder(const char *name, R (*func)(A...)) {
        AddFunc(name, _NTypePicker<R>::Type, (void (*)(A...))func);
    }
    
    template<class... A> void AddFunc(const char *name, NType retType, void (*func)(A...)) {
        
        bool error = false;
        int  count = CheckArgs(func, &error);
        
        if (error) {
            //some argument types are unsupported.
            return;
        }
        if (count > NFUNC_MAX_ARG_NUM) {
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
            info.retRetained = _NFuncRetRetained(retType, name);

            AddArgTypes(&info, func);
        }
        _NAddFunc(&info);
    }
    
    template<class A, class... B> int CheckArgs(void (*)(A, B...), bool *error) {
        NType type = _NTypePicker<A>::Type;
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

        info->argTypes[pos] = _NTypePicker<A>::Type;
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
