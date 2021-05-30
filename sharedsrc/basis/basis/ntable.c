#include "ntable.h"

NTable *NTableCreate(void) {
    return NULL;
}

NTable *NTableCopy(NTable *that) {
    return NULL;
}

void NTableSet(NTable *table, int64_t key, NObject *value) {
}

NObject *NTableGet(NTable *table, int64_t key) {
    return NULL;
}

void NTableRemove(NTable *table, int64_t key) {
}

bool NTableExist(NTable *table, int64_t key) {
    return false;
}

int NTableCount(NTable *table) {
    return 0;
}

NArray *NTableCopyPairs(NTable *table) {
    return NULL;
}
