#include "ntable.h"

static void WordTableRetainNodes(NWordTable *table) {
    scalist *nodes = atsort(&table->tree);
    for (int i = 0; i < slcount(nodes); ++i) {
        atnode *n = slget(nodes, i).asptr;
        
        if (table->conf.keyRetain) {NRetain(n->key  .asptr);}
        if (table->conf.valRetain) {NRetain(n->value.asptr);}
    }
}

static void WordTableReleaseNodes(NWordTable *table) {
    scalist *nodes = atsort(&table->tree);
    for (int i = 0; i < slcount(nodes); ++i) {
        atnode *n = slget(nodes, i).asptr;
        
        if (table->conf.keyRetain) {NRelease(n->key  .asptr);}
        if (table->conf.valRetain) {NRelease(n->value.asptr);}
    }
}

void _NWordTableInit(NWordTable *table, NWordTableConf *conf) {
    _NObjectInit(nsuperof(table));
    
    table->conf = *conf;
    atinit(&table->tree, table->conf.keyCompare);
    table->prefer = NULL;
}

void _NWordTableDeinit(NWordTable *table) {
    WordTableReleaseNodes(table);
    atdeinit(&table->tree);
    
    _NObjectDeinit(nsuperof(table));
}

NWordTable *NWordTableCreate(NWordTableConf *conf) {
    NWordTable *table = NAlloc(NWordTable, _NWordTableDeinit);
    _NWordTableInit(table, conf);
    return table;
}

NWordTable *NWordTableCopy(NWordTable *that) {
    if (!that) {
        return NULL;
    }
    
    NWordTable *table = NWordTableCreate(&that->conf);
    
    scalist *nodes = atsort(&that->tree);
    for (int i = 0; i < slcount(nodes); ++i) {
        atnode *n = slget(nodes, i).asptr;
        atset(&table->tree, n->key, n->value);
    }
    WordTableRetainNodes(table);
    
    return table;
}

void NWordTableSet(NWordTable *table, NWord key, NWord value) {
    if (!table) {
        return;
    }
    
    avltree *tree = &table->tree;
    atnode *node = atget(tree, pw(key.asPtr));
    if (node) {
        //the key already exists.
        if (table->conf.valRetain) {
            NRelease(node->value.asptr);
            NRetain(value.asPtr);
        }
        
        node->value = pw(value.asPtr);
    } else {
        //to insert new entry.
        if (table->conf.keyRetain) {NRetain(key  .asPtr);}
        if (table->conf.valRetain) {NRetain(value.asPtr);}
        
        atset(tree, pw(key.asPtr), pw(value.asPtr));
    }
    
    //NOTE: reset the preferred node when tree changed.
    table->prefer = NULL;
}

NWord NWordTableGet(NWordTable *table, NWord key) {
    NWord value = {0};
    
    if (!table) {
        return value;
    }
    
    //test the preferred node.
    word aim = pw(key.asPtr);
    if (table->prefer) {
        if (table->conf.keyCompare(table->prefer->key, aim) == 0) {
            value.asPtr = table->prefer->value.asptr;
            return value;
        }
    }
    
    //to traverse the tree.
    atnode *node = atget(&table->tree, aim);
    if (node) {
        value.asPtr = node->value.asptr;
        return value;
    }
    
    return value;
}

void NWordTableRemove(NWordTable *table, NWord key) {
    if (!table) {
        return;
    }
    
    //NOTE: reset the preferred node when tree changed.
    table->prefer = NULL;
    
    atnode node = atremove(&table->tree, pw(key.asPtr));
    
    if (table->conf.keyRetain) {NRelease(node.key  .asptr);}
    if (table->conf.valRetain) {NRelease(node.value.asptr);}
}

bool NWordTableExist(NWordTable *table, NWord key) {
    if (table) {
        return atget(&table->tree, pw(key.asPtr));
    }
    return false;
}

int NWordTableCount(NWordTable *table) {
    if (table) {
        return atcount(&table->tree);
    }
    return 0;
}

nstruct(NWordTableIterator, {
    NIterator   super;
    NWordTable *table;
    int         curr ;
});

static bool WordTableIteratorNext(NWordTableIterator *iterator) {
    scalist *nodes = atsort(&iterator->table->tree);
    
    if  ((iterator->curr + 1) < slcount(nodes)) {
        iterator->curr += 1;
        return true;
    }
    return false;
}

static NWord WordTableIteratorCurr(NWordTableIterator *iterator) {
    scalist *nodes = atsort(&iterator->table->tree);
    
    //NOTE: to store the current node as preferred.
    iterator->table->prefer = slget(nodes, iterator->curr).asptr;
    
    NWord key = {0};
    key.asPtr = iterator->table->prefer->key.asptr;
    return key;
}

NIterator *NWordTableKeys(NWordTable *table) {
    if (!table) {
        return NStoreIterator(NULL, 0);
    }

    NWordTableIterator iterator = {0};

    iterator.super.next = (NIteratorNext)WordTableIteratorNext;
    iterator.super.curr = (NIteratorCurr)WordTableIteratorCurr;
    iterator.table = table;
    iterator.curr  = -1;

    return NStoreIterator(&iterator, nsizeof(iterator));
}

static int nscmp(word a, word b) {
    return NStringCompare(a.asptr, b.asptr);
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
/**/    void TABLE##Remove(TABLE *table, KT key) {              \
/**/        NWord k = {0};                                      \
/**/        k.KM = key;                                         \
/**/                                                            \
/**/        NWordTableRemove(nsuperof(table), k);               \
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

GEN_TABLE(NTable   , pcmp , NRef     , true , asPtr, NRef, true, asPtr)
GEN_TABLE(NStrTable, nscmp, NString *, true , asPtr, NRef, true, asPtr)
GEN_TABLE(NIntTable, icmp , int      , false, asInt, NRef, true, asPtr)
