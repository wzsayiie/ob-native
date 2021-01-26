#include <cstdlib>
#include <cstring>
#include "nenviron.h"

struct _NStructInfo {
    const char *name;
    int size;
};

//the number of slots allocated each time.
static const int EACH_ALLOC_NUM = 32;

//the index "0" is reserved. the valid items start from "1".
static const int LIST_BEGIN = 1;

static _NStructInfo *sList = NULL;
static int sEnd = 0;
static int sConfine = 0;

static void _NAddStruct(_NStructInfo *info) {
    if (sEnd == sConfine) {
        sConfine += EACH_ALLOC_NUM;

        if (sEnd == 0) {
            sEnd = LIST_BEGIN;
        }

        auto newSize = sizeof(_NStructInfo) * sConfine;
        auto newList = (_NStructInfo *)realloc(sList, newSize);
        if (newList) {
            sList = newList;
        } else {
            return;
        }
    }

    //insertion sort here.
    int index = LIST_BEGIN;
    while (index < sEnd) {
        if (strcmp(sList[index].name, info->name) > 0) {
            break;
        }
        index += 1;
    }

    if (index < sEnd) {
        void *dst = sList + index + 1;
        void *src = sList + index;
        int   num = sEnd  - index;
        memmove(dst, src, num * sizeof(_NStructInfo));
    }
    sList[index] = *info;
    sEnd += 1;
}

nclink int NStructsBegin() {return LIST_BEGIN;}
nclink int NStructsEnd  () {return sEnd;}

nclink int NFindStruct(const char *name) {
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

static bool _NStructIndexable(int index) {
    return LIST_BEGIN <= index && index < sEnd;
}

nclink const char *NStructName(int index) {
    if (_NStructIndexable(index)) {
        return sList[index].name;
    }
    return NULL;
}

nclink int NStructSize(int index) {
    if (_NStructIndexable(index)) {
        return sList[index].size;
    }
    return 0;
}

struct _NStructAdder {
    _NStructAdder(const char *name, int size) {
        _NStructInfo info = {0};
        info.name = name;
        info.size = size;

        _NAddStruct(&info);
    }
};

#undef  nstruct
#define nstruct(n, ...) \
/**/    __nstruct(n, __VA_ARGS__); \
/**/    static _NStructAdder __adder_##n(#n, nisizeof(n))

#include "NEXPORT.h"
