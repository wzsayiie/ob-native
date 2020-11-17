#include <cstdint>
#include <cstring>

struct _NFuncInfo {
    const char *name;

    void *address;
    int   returnType;
    int   paramCount;
    int   paramTypes[8];
};

static const int   LIST_CAPACITY       = 256;
static _NFuncInfo _list[LIST_CAPACITY] = {0};
static int        _listCount           = 0;

static void _NAddFunc(_NFuncInfo *info) {
    if (_listCount == LIST_CAPACITY) {
        return;
    }

    //insertation sort here.
    int pos = 0;
    while (pos < _listCount) {
        if (strcmp(_list[pos].name, info->name) > 0) {
            break;
        }
        pos++;
    }

    if (pos < _listCount) {
        void *dst = _list + pos + 1;
        void *src = _list + pos;
        int   num = _listCount - pos;
        memmove(dst, src, num * sizeof(_NFuncInfo));
    }
    _list[pos] = *info;
    _listCount += 1;
}

extern "C" int NFuncCount() {
    return _listCount;
}

extern "C" int NFindFunc(const char *name) {
    if (name == NULL || *name == '\0') {
        return -1;
    }

    //binary search here.
    int begin = 0;
    int end = _listCount;

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
    return -1;
}

static _NFuncInfo *_Get(int fIndex) {
    static _NFuncInfo empty = {0};

    if (0 <= fIndex && fIndex < _listCount) {
        return _list + fIndex;
    } else {
        return &empty;
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

template<class> struct _NPickType          {static const int Type = 9;/* NTypePtr   */};
template<>      struct _NPickType<void   > {static const int Type = 1;/* NTypeVoid  */};
template<>      struct _NPickType<bool   > {static const int Type = 2;/* NTypeBool  */};
template<>      struct _NPickType<int8_t > {static const int Type = 3;/* NTypeInt8  */};
template<>      struct _NPickType<int16_t> {static const int Type = 4;/* NTypeInt16 */};
template<>      struct _NPickType<int32_t> {static const int Type = 5;/* NTypeInt32 */};
template<>      struct _NPickType<int64_t> {static const int Type = 6;/* NTypeInt64 */};
template<>      struct _NPickType<float  > {static const int Type = 7;/* NTypeFlt32 */};
template<>      struct _NPickType<double > {static const int Type = 8;/* NTypeFlt64 */};

struct _NFuncAdder {

    template<class R, class... A>
    _NFuncAdder(const char *name, R (*func)(A...)) {
        _NFuncInfo info = {0};

        info.name = name;
        info.address = func;
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

#define nfunc(r, n, p) __nfunc(r, n, p); static _NFuncAdder __adder_##n(#n, n)
#include "NEXPORT.h"
