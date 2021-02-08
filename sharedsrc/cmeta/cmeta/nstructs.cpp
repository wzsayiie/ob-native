#include <cstdlib>
#include <cstring>
#include "nenviron.h"

struct StructInfo {
    const char *name;
    int super;
    int size;
};

//the number of slots allocated each time.
static const int EACH_ALLOC_NUM = 64;

//the indexes at front are reserved.
static const int LIST_BEGIN = NTYPE_CUSTOM_PTR;
static_assert(LIST_BEGIN < EACH_ALLOC_NUM, "");

static StructInfo *sList = NULL;
static int sEnd = 0;
static int sConfine = 0;

static StructInfo *ReallocStructInfo(StructInfo *list, int count) {
    auto size = sizeof(StructInfo) * count;
    return (StructInfo *)realloc(list, size);
}

static StructInfo *EnsureStruct(const char *name) {
    if (sEnd == sConfine) {
        sConfine += EACH_ALLOC_NUM;
        sList = ReallocStructInfo(sList, sConfine);

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
        memmove(dst, src, num * sizeof(StructInfo));
    }
    StructInfo info = {0};
    info.name = name;
    sList[index] = info;
    sEnd += 1;

    //update reference data of all items.
    for (int n = LIST_BEGIN; n < sEnd; ++n) {
        StructInfo *item = sList + n;
        if (item->super != 0 && item->super >= index) {
            item->super += 1;
        }
    }

    return sList + index;
}

static int SearchStruct(const char *name) {
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

static void AddStruct(const char *name, const char *super, int size) {
    if (super) {
        EnsureStruct(super);
    }

    StructInfo *info = EnsureStruct(name);
    if (super) {
        info->super = SearchStruct(super);
    }
    info->size = size;
}

nclink int NStructsBegin() {return LIST_BEGIN;}
nclink int NStructsEnd  () {return sEnd;}

nclink int NFindStruct(const char *name) {
    if (name && *name) {
        return SearchStruct(name);
    }
    return 0;
}

static bool StructIndexable(int index) {
    return LIST_BEGIN <= index && index < sEnd;
}

nclink const char *NStructName(int index) {
    if (StructIndexable(index)) {
        return sList[index].name;
    }
    return NULL;
}

nclink int NStructSuper(int index) {
    if (StructIndexable(index)) {
        return sList[index].super;
    }
    return 0;
}

nclink int NStructSize(int index) {
    if (StructIndexable(index)) {
        return sList[index].size;
    }
    return 0;
}

struct StructAdder {
    StructAdder(const char *name, const char *super, int size) {
        AddStruct(name, super, size);
    }
};

#define __add_struct(n, z, s) static StructAdder __adder_##n(#n, z, s)

#undef  nstruct
#undef  nclass
#define nstruct(n,    ...) __nstruct(n,    __VA_ARGS__); __add_struct(n, NULL, nisizeof(n))
#define nclass( n, s, ...) __nclass (n, s, __VA_ARGS__); __add_struct(n, #s  , 0)

#include "NEXPORT.h"
