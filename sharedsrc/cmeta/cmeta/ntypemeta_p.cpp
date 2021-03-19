#include "ntypemeta_p.h"
#include "basis.h"
#include "binlist.h"
#include "cerpool.h"

const int INNER_INDEX_INVALID = -1;
const int INNER_INDEX_FIRST   =  0;

const int OUTER_INDEX_INVALID = 0;
const int OUTER_INDEX_FIRSY   = NTYPE_CUSTOM_REF;

struct StructEntry {
    const char *name;
    int super;
    int size;
};

static cerpool sEntryPool = {0};
static binlist sEntryList = {0};

static void InsertIfNeeded(const char *name) {
    nsynonce() {
        cpinit(&sEntryPool, nsizeof(StructEntry));
        blinit(&sEntryList, scmp);
    }
    
    int index = blindex(&sEntryList, pw(name));
    if (index != -1) {
        //the item already exists.
        return;
    }
    
    auto entry = (StructEntry *)cpborrow(&sEntryPool);
    index = blinsert(&sEntryList, pw(name), pw(entry));
    
    //NOTE: update field "super".
    for (int cnt = 0; cnt < blcount(&sEntryList); ++cnt) {
        auto entry = (StructEntry *)bloffset(&sEntryList, cnt).asptr;
        
        if (entry->super >= index) {
            entry->super += 1;
        }
    }
}

StructAdder::StructAdder(const char *name, const char *super, int size) {
    InsertIfNeeded(name);
    if (super) {
        InsertIfNeeded(super);
    }
    
    int currentIndex = blindex(&sEntryList, pw(name));
    int superIndex = INNER_INDEX_INVALID;
    if (super) {
        superIndex = blindex(&sEntryList, pw(super));
    }
    
    auto entry = (StructEntry *)bloffset(&sEntryList, currentIndex).asptr;
    entry->name  = name;
    entry->super = superIndex;
    entry->size  = size;
}

static int OuterIndexFromInner(int inner) {
    if (inner >= INNER_INDEX_FIRST) {
        return inner + (OUTER_INDEX_FIRSY - INNER_INDEX_FIRST);
    } else {
        return OUTER_INDEX_INVALID;
    }
}

static int InnerIndexFromOuter(int outer) {
    if (outer >= OUTER_INDEX_FIRSY) {
        return outer + (INNER_INDEX_FIRST - OUTER_INDEX_FIRSY);
    } else {
        return INNER_INDEX_INVALID;
    }
}

int StructsBegin() {
    return OuterIndexFromInner(0);
}

int StructsEnd() {
    int count = blcount(&sEntryList);
    return OuterIndexFromInner(count);
}

int FindStruct(const char *name) {
    int index = blindex(&sEntryList, pw(name));
    return OuterIndexFromInner(index);
}

static StructEntry *EntryAt(int index) {
    index = InnerIndexFromOuter(index);

    int count = blcount(&sEntryList);
    if (0 <= index && index < count) {
        return (StructEntry *)bloffset(&sEntryList, index).asptr;
    }
    return NULL;
}

const char *StructName(int index) {
    StructEntry *entry = EntryAt(index);
    if (entry) {
        return entry->name;
    }
    return NULL;
}

int StructSuper(int index) {
    StructEntry *entry = EntryAt(index);
    if (entry) {
        return OuterIndexFromInner(entry->super);
    }
    return 0;
}

int StructSize(int index) {
    StructEntry *entry = EntryAt(index);
    if (entry) {
        return entry->size;
    }
    return 0;
}
