#pragma once

#include "nstring.h"

nenum(__NTableKeyType) {
    __NTableKeyTypeString = 1,
    __NTableKeyTypeInt    = 2,
};

nenum(__NTableValueType) {
    __NTableValueTypeObject = 1,
    __NTableValueTypePOD    = 2,
};

nstruct(__NTable);

nclink __NTable *__NTableCreate(__NTableKeyType keyType, __NTableValueType valueType);
nclink __NTable *__NTableCopy  (__NTable *that);
nclink void      __NTableSet   (__NTable *self, NValue key, NValue value);
nclink NValue    __NTableGet   (__NTable *self, NValue key);
nclink int       __NTableCount (__NTable *self);

// nstruct(NTable);

// nfunc(NTable  *, NTableCreate, (void));
// nfunc(NTable  *, NTableCopy  , (NTable *that));
// nfunc(void     , NTableSet   , (NTable *self, NString *key, NObject *value));
// nfunc(NObject *, NTableGet   , (NTable *self, NString *key));
// nfunc(int      , NTableCount , (NTable *self));

// nstruct(NIntTable);

// nfunc(NIntTable *, NIntTableCreate, (void));
// nfunc(NIntTable *, NIntTableCopy  , (NIntTable *that));
// nfunc(void       , NIntTableSet   , (NIntTable *self, NString *key, int64_t value));
// nfunc(int64_t    , NIntTableGet   , (NIntTable *self, NString *key));
// nfunc(int        , NIntTableCount , (NIntTable *self));

// nstruct(NFltTable);

// nfunc(NFltTable *, NFltTableCreate, (void));
// nfunc(NFltTable *, NFltTableCopy  , (NFltTable *that));
// nfunc(void       , NFltTableSet   , (NFltTable *self, NString *key, double value));
// nfunc(double     , NFltTableGet   , (NFltTable *self, NString *key));
// nfunc(int        , NFltTableCount , (NFltTable *self));

// nstruct(NSparse);

// nfunc(NSparse *, NSparseCreate, (void));
// nfunc(NSparse *, NSparseCopy  , (NSparse *that));
// nfunc(void     , NSparseSet   , (NSparse *self, int64_t key, NObject *value));
// nfunc(NObject *, NSparseGet   , (NSparse *self, int64_t key));
// nfunc(int      , NSparseCount , (NSparse *self));

// nstruct(NIntSparse);

// nfunc(NIntSparse *, NIntSparseCreate, (void));
// nfunc(NIntSparse *, NIntSparseCopy  , (NIntSparse *that));
// nfunc(void        , NIntSparseSet   , (NIntSparse *self, int64_t key, int64_t value));
// nfunc(int64_t     , NIntSparseGet   , (NIntSparse *self, int64_t key));
// nfunc(int         , NIntSparseCount , (NIntSparse *self));

// nstruct(NFltSparse);

// nfunc(NFltSparse *, NFltSparseCreate, (void));
// nfunc(NFltSparse *, NFltSparseCopy  , (NFltSparse *that));
// nfunc(void        , NFltSparseSet   , (NFltSparse *self, int64_t key, double value));
// nfunc(double      , NFltSparseGet   , (NFltSparse *self, int64_t key));
// nfunc(int         , NFltSparseCount , (NFltSparse *self));