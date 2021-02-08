#include "ntable.h"

nclass(NWordTable, NObject, {
    NWordTableConf conf;
});

NWordTable *NWordTableCreate(NWordTableConf *conf) {
    return NULL;
}

NWordTable *NWordTableCopy(NWordTable *that) {
    return NULL;
}

void NWordTableSet(NWordTable *self, NWord key, NWord value) {
}

NWord NWordTableGet(NWordTable *self, NWord key) {
    NWord word = {0};
    return word;
}

int NWordTableCount(NWordTable *self) {
    return 0;
}

NIterator *NWordTableKeys(NWordTable *self) {
    return NULL;
}

static int ObjectComparer(NWord a, NWord b) {
    if (a.asPtr != b.asPtr) {
        return a.asPtr > b.asPtr ? 1 : -1;
    } else {
        return 0;
    }
}

static int StringComparer(NWord a, NWord b) {
    NString *x = a.asPtr;
    NString *y = b.asPtr;

    return NStringCompare(x, y);
}

static int Int64Comparer(NWord a, NWord b) {
    if (a.asInt64 != b.asInt64) {
        return a.asInt64 > b.asInt64 ? 1 : -1;
    } else {
        return 0;
    }
}

#define GEN_TABLE(TABLE, K_C, K_T, K_R, K_M, V_T, V_R, V_M)     \
/**/                                                            \
/**/    TABLE *TABLE##Create(void) {                            \
/**/        NWordTableConf conf = {0};                          \
/**/                                                            \
/**/        conf.keyCompare  = K_C;                             \
/**/        conf.keyRetain   = K_R;                             \
/**/        conf.keySize     = nisizeof(K_T);                   \
/**/        conf.valueRetain = V_R;                             \
/**/        conf.valueSize   = nisizeof(V_T);                   \
/**/                                                            \
/**/        return (TABLE *)NWordTableCreate(&conf);            \
/**/    }                                                       \
/**/    TABLE *TABLE##Copy(TABLE *that) {                       \
/**/        return (TABLE *)NWordTableCopy((NWordTable *)that); \
/**/    }                                                       \
/**/    void TABLE##Set(TABLE *self, K_T key, V_T value) {      \
/**/        NWord k = {0};                                      \
/**/        NWord v = {0};                                      \
/**/                                                            \
/**/        k.K_M = key;                                        \
/**/        v.V_M = value;                                      \
/**/                                                            \
/**/        NWordTableSet((NWordTable *)self, k, v);            \
/**/    }                                                       \
/**/    V_T TABLE##Get(TABLE *self, K_T key) {                  \
/**/        NWord k = {0};                                      \
/**/        k.K_M = key;                                        \
/**/                                                            \
/**/        NWord v = NWordTableGet((NWordTable *)self, k);     \
/**/        return v.V_M;                                       \
/**/    }                                                       \
/**/    int TABLE##Count(TABLE *self) {                         \
/**/        return NWordTableCount((NWordTable *)self);         \
/**/    }                                                       \
/**/    NIterator *TABLE##Keys(TABLE *self) {                   \
/**/        return NWordTableKeys((NWordTable *)self);          \
/**/    }

GEN_TABLE(NTable   , ObjectComparer, NObject *, true , asPtr  , NObject *, true, asPtr)
GEN_TABLE(NStrTable, StringComparer, NString *, true , asPtr  , NObject *, true, asPtr)
GEN_TABLE(NIntTable, Int64Comparer , int64_t  , false, asInt64, NObject *, true, asPtr)
