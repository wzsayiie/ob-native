#pragma once

#include "avltree.h"
#include "narray.h"

nstruct(NTable, {
    avltree tree;
});

nfunc(NTable  *, NTableCreate, (void));
nfunc(NTable  *, NTableCopy  , (NTable *that));
nfunc(void     , NTableSet   , (NTable *table, int64_t key, NObject *value));
nfunc(NObject *, NTableGet   , (NTable *table, int64_t key));
nfunc(void     , NTableRemove, (NTable *table, int64_t key));
nfunc(bool     , NTableExist , (NTable *table, int64_t key));
nfunc(int      , NTableCount , (NTable *table));

nstruct(NTablePair, {
    int64_t  key;
    NObject *value;
});

nfunc(NArray *, NTableCopyPairs, (NTable *table));
