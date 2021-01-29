#include <cstdlib>
#include <cstring>
#include "nenviron.h"

struct _NStructInfo {
    const char *name;
    int super;
    int size;
};

//the number of slots allocated each time.
static const int EACH_ALLOC_NUM = 64;

//the indexes at front are reserved.
static const int LIST_BEGIN = NTypeCustomPtr;
static_assert(LIST_BEGIN < EACH_ALLOC_NUM, "");

static _NStructInfo *sList = NULL;
static int sEnd = 0;
static int sConfine = 0;

static _NStructInfo *_NReallocStructInfo(_NStructInfo *list, int count) {
    auto size = sizeof(_NStructInfo) * count;
    return (_NStructInfo *)realloc(list, size);
}

static _NStructInfo *_NEnsureStruct(const char *name) {
    if (sEnd == sConfine) {
        sConfine += EACH_ALLOC_NUM;
        sList = _NReallocStructInfo(sList, sConfine);

        if (sEnd == 0) {
            sEnd = LIST_BEGIN;
        }
    }

    //insertion sort followed.
    int begin = LIST_BEGIN;
    int end   = sEnd;
    int index = (begin + end) / 2;
    while (begin < end) {
        int result = strcmp(name, sList[index].name);
        if (result == 0) {
            //the item already exists.
            return sList + index;
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
        memmove(dst, src, num * sizeof(_NStructInfo));
    }
    _NStructInfo info = {0};
    info.name = name;
    sList[index] = info;
    sEnd += 1;

    //update reference data of all items.
    for (int n = LIST_BEGIN; n < sEnd; ++n) {
        _NStructInfo *item = sList + n;
        if (item->super != 0 && item->super >= index) {
            item->super += 1;
        }
    }

    return sList + index;
}

static int _NSearchStruct(const char *name) {
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

static void _NAddStruct(const char *name, const char *super, int size) {
    if (super) {
        _NEnsureStruct(super);
    }

    _NStructInfo *info = _NEnsureStruct(name);
    if (super) {
        info->super = _NSearchStruct(super);
    }
    info->size = size;
}

nclink int NStructsBegin() {return LIST_BEGIN;}
nclink int NStructsEnd  () {return sEnd;}

nclink int NFindStruct(const char *name) {
    if (name && *name) {
        return _NSearchStruct(name);
    }
    return 0;
}

static bool _NStructIndexable(int index) {
    return LIST_BEGIN <= index && index < sEnd;
}

nclink const char *NStructName(int index) {
    if (_NStructIndexable(index)) {
        return sList[index].name;
    }
    return NULL;
}

nclink int NStructSuper(int index) {
    if (_NStructIndexable(index)) {
        return sList[index].super;
    }
    return 0;
}

nclink int NStructSize(int index) {
    if (_NStructIndexable(index)) {
        return sList[index].size;
    }
    return 0;
}

struct _NStructAdder {
    _NStructAdder(const char *name, const char *super, int size) {
        _NAddStruct(name, super, size);
    }
};

#define __add_struct(n, z, s) static _NStructAdder __adder_##n(#n, z, s)

#undef  nstruct
#undef  nclass
#define nstruct(n,    ...) __nstruct(n,    __VA_ARGS__); __add_struct(n, NULL, nisizeof(n))
#define nclass( n, s, ...) __nclass (n, s, __VA_ARGS__); __add_struct(n, #s  , 0)

#include "NEXPORT.h"
