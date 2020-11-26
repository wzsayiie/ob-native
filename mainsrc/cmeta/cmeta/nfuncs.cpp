#include <cstring>
#include "ndef.h"

struct _NFuncInfo {
    const char *name;

    void *address;
    NType returnType;
    int   paramCount;
    NType paramTypes[8];
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

extern "C" int NFuncsIndexBegin() {return LIST_BEGIN;}
extern "C" int NFuncsIndexEnd  () {return _listEnd  ;}

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

static _NFuncInfo *_Get(int fIndex) {
    if (LIST_BEGIN <= fIndex && fIndex < _listEnd) {
        return _list + fIndex;
    } else {
        return _list;
    }
}

extern "C" const char *NFuncName      (int i) {return _Get(i)->name      ;}
extern "C" void       *NFuncAddress   (int i) {return _Get(i)->address   ;}
extern "C" int         NFuncReturnType(int i) {return _Get(i)->returnType;}
extern "C" int         NFuncParamCount(int i) {return _Get(i)->paramCount;}

extern "C" int NFuncParamType(int fIndex, int pIndex) {
    _NFuncInfo *info = _Get(fIndex);
    if (0 <= pIndex && pIndex < info->paramCount) {
        return info->paramTypes[pIndex];
    }
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

        info.name = name;
        info.address = (void *)func;
        info.returnType = _NPickType<R>::Type;
        SetParamTypes(&info, func);

        _NAddFunc(&info);
    }

    template<class R, class A, class... B>
    void SetParamTypes(_NFuncInfo *info, R (*)(A, B...)) {
        int index = (info->paramCount)++;
        info->paramTypes[index] = _NPickType<A>::Type;
        SetParamTypes(info, (R (*)(B...))NULL);
    }

    template<class R>
    void SetParamTypes(_NFuncInfo *info, R (*)()) {
    }
};

#undef  nfunc
#define nfunc(r, n, p) __nfunc(r, n, p); static _NFuncAdder __adder_##n(#n, n)
#include "NEXPORT.h"
