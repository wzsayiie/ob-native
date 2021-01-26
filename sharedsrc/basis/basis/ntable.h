#pragma once

#include "nstring.h"
#include "npod.h"

nstruct(NWordTableConf) {
    int(*keyCompare)(NWord a, NWord b);
    bool keyRetain;
    int  keySize;

    bool valueRetain;
    int  valueSize;
};

nclass(NWordTable, NObject);

nclink NWordTable *NWordTableCreate(NWordTableConf *conf);
nclink NWordTable *NWordTableCopy  (NWordTable *that);
nclink void        NWordTableSet   (NWordTable *self, NWord key, NWord value);
nclink NWord       NWordTableGet   (NWordTable *self, NWord key);
nclink int         NWordTableCount (NWordTable *self);
nclink NIterator  *NWordTableKeys  (NWordTable *self);

nclass(NTable, NWordTable);

nfunc(NTable    *, NTableCreate, (void));
nfunc(NTable    *, NTableCopy  , (NTable *that));
nfunc(void       , NTableSet   , (NTable *self, NObject *key, NObject *value));
nfunc(NObject   *, NTableGet   , (NTable *self, NObject *key));
nfunc(int        , NTableCount , (NTable *self));
nfunc(NIterator *, NTableKeys  , (NTable *self));

nclass(NStrTable, NWordTable);

nfunc(NStrTable *, NStrTableCreate, (void));
nfunc(NStrTable *, NStrTableCopy  , (NStrTable *that));
nfunc(void       , NStrTableSet   , (NStrTable *self, NString *key, NObject *value));
nfunc(NObject   *, NStrTableGet   , (NStrTable *self, NString *key));
nfunc(int        , NStrTableCount , (NStrTable *self));
nfunc(NIterator *, NStrTableKeys  , (NStrTable *self));

nclass(NIntTable, NWordTable);

nfunc(NIntTable *, NIntTableCreate, (void));
nfunc(NIntTable *, NIntTableCopy  , (NIntTable *that));
nfunc(void       , NIntTableSet   , (NIntTable *self, int64_t key, NObject *value));
nfunc(NObject   *, NIntTableGet   , (NIntTable *self, int64_t key));
nfunc(int        , NIntTableCount , (NIntTable *self));
nfunc(NIterator *, NIntTableKeys  , (NIntTable *self));
