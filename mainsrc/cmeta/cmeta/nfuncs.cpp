#include <cstring>
#include "ndef.h"

#ifndef NFUNC_MAX_ARG_NUM
#define NFUNC_MAX_ARG_NUM 6
#endif

struct _NFuncInfo {
    const char *name;

    void *address;
    NType retType;
    int   argCount;
    NType argTypes[NFUNC_MAX_ARG_NUM];
};

static const int   LIST_BEGIN     = 1;
static const int   LIST_END       = 256;
static _NFuncInfo _list[LIST_END] = {0};
static int        _listEnd        = 0;

static void _NAddFunc(_NFuncInfo *info) {
    if (_listEnd == LIST_END) {
        return;
    }

    //insertion sort here.
    int pos = LIST_BEGIN;
    while (pos < _listEnd) {
        if (strcmp(_list[pos].name, info->name) > 0) {
            break;
        }
        pos++;
    }

    if (pos < _listEnd) {
        void *dst = _list + pos + 1;
        void *src = _list + pos;
        int   num = _listEnd - pos;
        memmove(dst, src, num * sizeof(_NFuncInfo));
    }
    _list[pos] = *info;
    _listEnd += 1;
}

extern "C" int NFuncsBegin() {return LIST_BEGIN;}
extern "C" int NFuncsEnd  () {return _listEnd  ;}

extern "C" int NFindFunc(const char *name) {
    if (name == NULL || *name == '\0') {
        return 0;
    }

    //binary search here.
    int begin = LIST_BEGIN;
    int end   = _listEnd  ;

    while (begin < end) {
        int center = (begin + end) / 2;
        int result = strcmp(name, _list[center].name);

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
    if (LIST_BEGIN <= fPos && fPos < _listEnd) {
        return _list + fPos;
    } else {
        return _list;
    }
}

extern "C" const char *NFuncName    (int i) {return _NFuncGetInfo(i)->name    ;}
extern "C" void       *NFuncAddress (int i) {return _NFuncGetInfo(i)->address ;}
extern "C" int         NFuncRetType (int i) {return _NFuncGetInfo(i)->retType ;}
extern "C" int         NFuncArgCount(int i) {return _NFuncGetInfo(i)->argCount;}

extern "C" NType NFuncArgType(int fPos, int aPos) {
    _NFuncInfo *info = _NFuncGetInfo(fPos);
    if (0 <= aPos && aPos < info->argCount) {
        return info->argTypes[aPos];
    }
    return 0;
}

extern "C" int64_t NCallFunc(int fPos, int argCount, int64_t *args) {
    return 0;
}

template<class> struct _NPickType          {static const NType Type = NTypePtr  ;};
template<>      struct _NPickType<void   > {static const NType Type = NTypeVoid ;};
template<>      struct _NPickType<bool   > {static const NType Type = NTypeBool ;};
template<>      struct _NPickType<int8_t > {static const NType Type = NTypeInt8 ;};
template<>      struct _NPickType<int16_t> {static const NType Type = NTypeInt16;};
template<>      struct _NPickType<int32_t> {static const NType Type = NTypeInt32;};
template<>      struct _NPickType<int64_t> {static const NType Type = NTypeInt64;};
template<>      struct _NPickType<float  > {static const NType Type = NTypeFlt32;};
template<>      struct _NPickType<double > {static const NType Type = NTypeFlt64;};

struct _NFuncAdder {

    template<class R, class... A>
    _NFuncAdder(const char *name, R (*func)(A...)) {
        _NFuncInfo info = {0};

        info.name    = name;
        info.address = (void *)func;
        info.retType = _NPickType<R>::Type;

        AppendArgType(&info, func);

        _NAddFunc(&info);
    }

    template<class R, class A, class... B>
    void AppendArgType(_NFuncInfo *info, R (*)(A, B...)) {
        int pos = (info->argCount)++;
        info->argTypes[pos] = _NPickType<A>::Type;
        AppendArgType(info, (R (*)(B...))NULL);
    }

    template<class R>
    void AppendArgType(_NFuncInfo *info, R (*)()) {
    }
};

#undef  nfunc
#define nfunc(r, n, p) __nfunc(r, n, p); static _NFuncAdder __adder_##n(#n, n)
#include "NEXPORT.h"
