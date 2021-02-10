#include "ntable.h"

void _NWordTableInit(NWordTable *table, NWordTableConf *conf) {
}

void _NWordTableDeinit(NWordTable *table) {
}

NWordTable *NWordTableCreate(NWordTableConf *conf) {
    return NULL;
}

NWordTable *NWordTableCopy(NWordTable *that) {
    return NULL;
}

void NWordTableSet(NWordTable *table, NWord key, NWord value) {
}

NWord NWordTableGet(NWordTable *table, NWord key) {
    NWord val = {0};
    return val;
}

bool NWordTableExist(NWordTable *table, NWord key) {
    return false;
}

int NWordTableCount(NWordTable *table) {
    return 0;
}

NIterator *NWordTableKeys(NWordTable *table) {
    return NULL;
}

static int RefComparer(NWord a, NWord b) {
    if (a.asPtr != b.asPtr) {
        return a.asPtr > b.asPtr ? 1 : -1;
    } else {
        return 0;
    }
}

static int StrComparer(NWord a, NWord b) {
    return NStringCompare(a.asPtr, b.asPtr);
}

static int IntComparer(NWord a, NWord b) {
    if (a.asInt64 != b.asInt64) {
        return a.asInt64 > b.asInt64 ? 1 : -1;
    } else {
        return 0;
    }
}

#define GEN_TABLE(TABLE, KC, KT, KR, KM, VT, VR, VM)            \
/**/                                                            \
/**/    void _##TABLE##Init(TABLE *table) {                     \
/**/        NWordTableConf conf = {0};                          \
/**/        conf.keyCompare = KC;                               \
/**/        conf.keyRetain  = KR;                               \
/**/        conf.valRetain  = VR;                               \
/**/                                                            \
/**/        _NWordTableInit(nsuperof(table), &conf);            \
/**/    }                                                       \
/**/    void _##TABLE##Deinit(TABLE *table) {                   \
/**/        _NWordTableDeinit(nsuperof(table));                 \
/**/    }                                                       \
/**/    TABLE *TABLE##Create(void) {                            \
/**/        TABLE *table = NAlloc(TABLE, _##TABLE##Deinit);     \
/**/        _##TABLE##Init(table);                              \
/**/        return table;                                       \
/**/    }                                                       \
/**/    TABLE *TABLE##Copy(TABLE *that) {                       \
/**/        return (TABLE *)NWordTableCopy(nsuperof(that));     \
/**/    }                                                       \
/**/    void TABLE##Set(TABLE *table, KT key, VT value) {       \
/**/        NWord k = {0};                                      \
/**/        NWord v = {0};                                      \
/**/                                                            \
/**/        k.KM = key;                                         \
/**/        v.VM = value;                                       \
/**/                                                            \
/**/        NWordTableSet(nsuperof(table), k, v);               \
/**/    }                                                       \
/**/    VT TABLE##Get(TABLE *table, KT key) {                   \
/**/        NWord k = {0};                                      \
/**/        k.KM = key;                                         \
/**/                                                            \
/**/        NWord v = NWordTableGet(nsuperof(table), k);        \
/**/        return v.VM;                                        \
/**/    }                                                       \
/**/    bool TABLE##Exist(TABLE *table, KT key) {               \
/**/        NWord k = {0};                                      \
/**/        k.KM = key;                                         \
/**/                                                            \
/**/        return NWordTableExist(nsuperof(table), k);         \
/**/    }                                                       \
/**/    int TABLE##Count(TABLE *table) {                        \
/**/        return NWordTableCount(nsuperof(table));            \
/**/    }                                                       \
/**/    NIterator *TABLE##Keys(TABLE *table) {                  \
/**/        return NWordTableKeys(nsuperof(table));             \
/**/    }

GEN_TABLE(NTable   , RefComparer, NRef     , true , asPtr, NRef, true, asPtr)
GEN_TABLE(NStrTable, StrComparer, NString *, true , asPtr, NRef, true, asPtr)
GEN_TABLE(NIntTable, IntComparer, int      , false, asInt, NRef, true, asPtr)
