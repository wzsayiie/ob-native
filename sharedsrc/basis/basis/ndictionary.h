#pragma once

#include "avltree.h"
#include "narray.h"
#include "nstring.h"

nstruct(NDictionary, {
    avltree tree;
});

nfunc(NDictionary *, NDictionaryCreate, (void));
nfunc(NDictionary *, NDictionaryCopy  , (NDictionary *that));
nfunc(void         , NDictionarySet   , (NDictionary *dictionary, NString *key, NObject *value));
nfunc(NObject     *, NDictionaryGet   , (NDictionary *dictionary, NString *key));
nfunc(void         , NDictionaryRemove, (NDictionary *dictionary, NString *key));
nfunc(bool         , NDictionaryExist , (NDictionary *dictionary, NString *key));
nfunc(int          , NDictionaryCount , (NDictionary *dictionary));

nstruct(NDictionaryPair, {
    NString *key;
    NObject *value;
});

nfunc(NArray *, NDictionaryCopyPairs, (NDictionary *dictionary));
