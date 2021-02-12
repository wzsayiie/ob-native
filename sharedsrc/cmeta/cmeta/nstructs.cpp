#include <cstdlib>
#include <cstring>
#include "cerpool.h"
#include "binlist.h"
#include "nenviron.h"

//the indexes at front are reserved.
static const int INDEX_OFFSET = NTYPE_CUSTOM_PTR;

struct StructInfo {
    const char *name;
    int super;
    int size;
};

static bool    sNeedInit = true;
static cerpool sItemPool = {0};
static binlist sInfoList = {0};

static void EnsureInfo(const char *name) {
    if (sNeedInit) {
        cpinit(&sItemPool, nsizeof(StructInfo));
        blinit(&sInfoList, scmp);
        sNeedInit = false;
    }
    
    int index = blindex(&sInfoList, pw(name));
    if (index != -1) {
        //the item already exists.
        return;
    }
    
    auto item = (StructInfo *)cpborrow(&sItemPool);
    index = blinsert(&sInfoList, pw(name), pw(item));
    
    //NOTE: update field "super".
    for (int i = 0; i < blcount(&sInfoList); ++i) {
        word currItem = bloffset(&sInfoList, i);
        auto currInfo = (StructInfo *)currItem.asptr;
        
        if (currInfo->super >= index + INDEX_OFFSET) {
            currInfo->super += 1;
        }
    }
}

static int InfoCount() {
    return blcount(&sInfoList);
}

static int IndicateInfo(const char *name) {
    int index = blindex(&sInfoList, pw(name));
    if (index != -1) {
        return INDEX_OFFSET + index;
    } else {
        return 0;
    }
}

static StructInfo *GetInfo(int index) {
    index -= INDEX_OFFSET;
    if (0 <= index && index < InfoCount()) {
        word item = bloffset(&sInfoList, index);
        return (StructInfo *)item.asptr;
    } else {
        return NULL;
    }
}

static void AddStruct(const char *name, const char *super, int size) {
    EnsureInfo(name);
    if (super) {
        EnsureInfo(super);
    }
    
    int infoIndex = IndicateInfo(name);
    int superIndex = 0;
    if (super) {
        superIndex = IndicateInfo(super);
    }
    
    StructInfo *info = GetInfo(infoIndex);
    info->name  = name;
    info->super = superIndex;
    info->size  = size;
}

nclink int NStructsBegin() {return INDEX_OFFSET;}
nclink int NStructsEnd  () {return INDEX_OFFSET + InfoCount();}

nclink int NFindStruct(const char *name) {
    if (name && *name) {
        return IndicateInfo(name);
    }
    return 0;
}

nclink const char *NStructName (int i) {auto s = GetInfo(i); return s ? s->name  : NULL;}
nclink int         NStructSuper(int i) {auto s = GetInfo(i); return s ? s->super : 0   ;}
nclink int         NStructSize (int i) {auto s = GetInfo(i); return s ? s->size  : 0   ;}

struct StructAdder {
    StructAdder(const char *name, const char *super, int size) {
        AddStruct(name, super, size);
    }
};

#define __add_struct(n, z, s) static StructAdder __adder_##n(#n, z, s)

#undef  nstruct
#undef  nclass
#define nstruct(n,    ...) __nstruct(n,    __VA_ARGS__); __add_struct(n, NULL, nsizeof(n))
#define nclass( n, s, ...) __nclass (n, s, __VA_ARGS__); __add_struct(n, #s  , nsizeof(n))

#include "NEXPORT.h"
