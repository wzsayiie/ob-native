#include "nfuncmeta_p.h"
#include <cstring>
#include "basis.h"
#include "binlist.h"
#include "cerpool.h"
#include "ntypemeta_p.h"

static cerpool sEntryPool = {0};
static binlist sEntryList = {0};

bool FuncAdder::RetRetain(NType retType, const char *funcName) {
    if (retType >= NTYPE_OBJECT) {
        if (strstr(funcName, "Create")) {return true;}
        if (strstr(funcName, "Copy"  )) {return true;}
        if (strstr(funcName, "Retain")) {return true;}
    }
    return false;
}

void FuncAdder::AddEntry(FuncEntry *entry) {
    nsynonce() {
        cpinit(&sEntryPool, nsizeof(FuncEntry));
        blinit(&sEntryList, scmp);

        //occupy index 0, and it as a reserved position.
        blinsert(&sEntryList, pw("!"), pw(NULL));
    }
    
    auto heap = (FuncEntry *)cpborrow(&sEntryPool);
    *heap = *entry;
    blinsert(&sEntryList, pw(heap->funcName), pw(heap));
}

int FuncsBegin() {
    //index 0 is reserved.
    return 1;
}

int FuncsEnd() {
    return blcount(&sEntryList);
}

int FindFunc(const char *name) {
    int index = blindex(&sEntryList, pw(name));
    if (index != -1) {
        return index;
    }
    return 0;
}

static FuncEntry *GetEntry(int index) {
    int count = blcount(&sEntryList);
    if (1 <= index && index < count) {
        return (FuncEntry *)bloffset(&sEntryList, index).asptr;
    }
    return NULL;
}

const char *FuncName(int index) {
    FuncEntry *entry = GetEntry(index);
    if (entry) {
        return entry->funcName;
    }
    return NULL;
}

void *FuncAddress(int index) {
    FuncEntry *entry = GetEntry(index);
    if (entry) {
        return entry->funcAddr;
    }
    return NULL;
}

NType FuncRetType(int index) {
    FuncEntry *entry = GetEntry(index);
    if (!entry) {
        return 0;
    }

    if (entry->retType == 0) {
        entry->retType = FindType(entry->retName);
    }
    return entry->retType;
}

bool FuncRetRetain(int index) {
    FuncEntry *entry = GetEntry(index);
    if (entry) {
        return entry->retRetain;
    }
    return false;
}

int FuncArgCount(int index) {
    FuncEntry *entry = GetEntry(index);
    if (entry) {
        return entry->argCount;
    }
    return 0;
}

NType FuncArgType(int index, int argIndex) {
    FuncEntry *entry = GetEntry(index);
    if (!entry) {
        return 0;
    }

    if (!(0 <= argIndex && argIndex < entry->argCount)) {
        return 0;
    }

    if (entry->argTypes[argIndex] == 0) {
        const char *name = entry->argNames[argIndex];
        entry->argTypes[argIndex] = FindType(name);
    }
    return entry->argTypes[argIndex];
}
