#pragma once

#include "nstring.h"
#include "npod.h"

nstruct(NWordTableConf, {
    int(*keyCompare)(NWord a, NWord b);
    bool keyRetain;
    int  keySize;

    bool valueRetain;
    int  valueSize;
});

nclass(NWordTable, NObject, {
    union {
        NObject Object;
        NObject Super ;
    };

    NWordTableConf conf;
});

void _NWordTableInit  (NWordTable *table, NWordTableConf *conf);
void _NWordTableDeinit(NWordTable *table);

nclink NWordTable *NWordTableCreate(NWordTableConf *conf);
nclink NWordTable *NWordTableCopy  (NWordTable *that );
nclink void        NWordTableSet   (NWordTable *table, NWord key, NWord value);
nclink NWord       NWordTableGet   (NWordTable *table, NWord key);
nclink int         NWordTableCount (NWordTable *table);
nclink NIterator  *NWordTableKeys  (NWordTable *table);

nclass(NTable, NWordTable, {
    union {
        NObject    Object   ;
        NWordTable WordTable;
        NWordTable Super    ;
    };
});

nfunc(NTable    *, NTableCreate, (void));
nfunc(NTable    *, NTableCopy  , (NTable *that ));
nfunc(void       , NTableSet   , (NTable *table, NRef key, NRef value));
nfunc(NRef       , NTableGet   , (NTable *table, NRef key));
nfunc(int        , NTableCount , (NTable *table));
nfunc(NIterator *, NTableKeys  , (NTable *table));

nclass(NStrTable, NWordTable, {
    union {
        NObject    Object   ;
        NWordTable WordTable;
        NWordTable Super    ;
    };
});

nfunc(NStrTable *, NStrTableCreate, (void));
nfunc(NStrTable *, NStrTableCopy  , (NStrTable *that ));
nfunc(void       , NStrTableSet   , (NStrTable *table, NString *key, NRef value));
nfunc(NRef       , NStrTableGet   , (NStrTable *table, NString *key));
nfunc(int        , NStrTableCount , (NStrTable *table));
nfunc(NIterator *, NStrTableKeys  , (NStrTable *table));

nclass(NIntTable, NWordTable, {
    union {
        NObject    Object   ;
        NWordTable WordTable;
        NWordTable Super    ;
    };
});

nfunc(NIntTable *, NIntTableCreate, (void));
nfunc(NIntTable *, NIntTableCopy  , (NIntTable *that ));
nfunc(void       , NIntTableSet   , (NIntTable *table, int64_t key, NRef value));
nfunc(NRef       , NIntTableGet   , (NIntTable *table, int64_t key));
nfunc(int        , NIntTableCount , (NIntTable *table));
nfunc(NIterator *, NIntTableKeys  , (NIntTable *table));
