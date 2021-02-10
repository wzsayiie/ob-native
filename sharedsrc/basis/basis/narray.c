#include "narray.h"

static void WordArrayRetainItems(NWordArray *array) {
    scalist *list = &array->list;
    for (int i = 0; i < slcount(list); ++i) {
        word item = slget(list, i);
        NRetain(item.asptr);
    }
}

static void WordArrayReleaseItems(NWordArray *array) {
    scalist *list = &array->list;
    for (int i = 0; i < slcount(list); ++i) {
        word item = slget(list, i);
        NRelease(item.asptr);
    }
}

void _NWordArrayInit(NWordArray *array, NWordArrayConf *conf) {
    _NObjectInit(nsuperof(array));

    array->conf = *conf;
    slinit(&array->list);
}

void _NWordArrayDeinit(NWordArray *array) {
    if (array->conf.retain) {
        WordArrayReleaseItems(array);
    }
    sldeinit(&array->list);
    
    _NObjectDeinit(nsuperof(array));
}

NWordArray *NWordArrayCreate(NWordArrayConf *conf) {
    NWordArray *array = NAlloc(NWordArray, _NWordArrayDeinit);
    _NWordArrayInit(array, conf);
    return array;
}

NWordArray *NWordArrayCopy(NWordArray *that) {
    if (!that) {
        return NULL;
    }

    NWordArray *array = NWordArrayCreate(&that->conf);

    scalist *tlist = &that ->list;
    scalist *alist = &array->list;
    for (int i = 0; i < slcount(tlist); ++i) {
        word item = slget(tlist, i);
        slpush(alist, item);
    }
    if (array->conf.retain) {
        WordArrayRetainItems(array);
    }

    return array;
}

int NWordArrayCount(NWordArray *array) {
    if (array) {
        return slcount(&array->list);
    }
    return 0;
}

nstruct(NWordArrayIterator, {
    NIterator   super ;
    NWordArray *array ;
    int         cursor;
});

static bool WordArrayIteratorNext(NWordArrayIterator *iterator) {
    return (iterator->cursor) < slcount(&(iterator->array->list));
}

static void *WordArrayIteratorGet(NWordArrayIterator *iterator) {
    static nthreadlocal word item = {0};

    item = slget(&(iterator->array->list), (iterator->cursor)++);
    return &item;
}

NIterator *NWordArrayItems(NWordArray *array) {
    if (!array) {
        return NStoreIterator(NULL, 0);
    }

    NWordArrayIterator iterator = {0};

    iterator.super.next = (NIteratorNextFunc)WordArrayIteratorNext;
    iterator.super.get  = (NIteratorGetFunc )WordArrayIteratorGet ;
    iterator.array  = array;
    iterator.cursor = 0;

    return NStoreIterator(&iterator, nsizeof(iterator));
}

void NWordArrayPush(NWordArray *array, NWord item) {
    if (!array) {
        return;
    }
    
    if (array->conf.retain) {
        NRetain(item.asPtr);
    }
    scalist *list = &array->list;
    slpush(list, pw(item.asPtr));
}

void NWordArrayPop(NWordArray *array) {
    if (!array) {
        return;
    }
    
    scalist *list = &array->list;
    if (slcount(list) == 0) {
        return;
    }
    
    word item = slpop(list);
    if (array->conf.retain) {
        NRelease(item.asptr);
    }
}

void NWordArrayInsert(NWordArray *array, int index, NWord item) {
    if (!array) {
        return;
    }
    
    scalist *list = &array->list;
    if (!(0 <= index && index <= slcount(list))) {
        return;
    }

    if (array->conf.retain) {
        NRetain(item.asPtr);
    }
    slinsert(list, index, pw(item.asPtr));
}

void NWordArrayRemove(NWordArray *array, int index) {
    if (!array) {
        return;
    }
    
    scalist *list = &array->list;
    if (!(0 <= index && index < slcount(list))) {
        return;
    }

    word item = slremove(list, index);
    if (array->conf.retain) {
        NRelease(item.asptr);
    }
}

void NWordArraySet(NWordArray *array, int index, NWord item) {
    if (!array) {
        return;
    }
    
    scalist *list = &array->list;
    if (!(0 <= index && index < slcount(list))) {
        return;
    }
    
    if (array->conf.retain) {
        word old = slget(list, index);
        NRelease(old.asptr);
        
        NRetain(item.asPtr);
    }
    slset(list, index, pw(item.asPtr));
}

NWord NWordArrayGet(NWordArray *array, int index) {
    NWord ret = {0};
    
    if (!array) {
        return ret;
    }
    
    scalist *list = &array->list;
    if (!(0 <= index && index < slcount(list))) {
        return ret;
    }
    
    word item = slget(list, index);
    ret.asPtr = item.asptr;
    return ret;
}

#define GEN_ARRAY(ARRAY, TYPE, RETAIN, MEMBER)                      \
/**/                                                                \
/**/    void _##ARRAY##Init(ARRAY *array) {                         \
/**/        NWordArrayConf conf = {0};                              \
/**/        conf.retain = RETAIN;                                   \
/**/        _NWordArrayInit(nsuperof(array), &conf);                \
/**/    }                                                           \
/**/    void _##ARRAY##Deinit(ARRAY *array) {                       \
/**/        _NWordArrayDeinit(nsuperof(array));                     \
/**/    }                                                           \
/**/    ARRAY *ARRAY##Create(void) {                                \
/**/        ARRAY *array = NAlloc(ARRAY, _##ARRAY##Deinit);         \
/**/        _##ARRAY##Init(array);                                  \
/**/        return array;                                           \
/**/    }                                                           \
/**/    ARRAY *ARRAY##Copy(ARRAY *array) {                          \
/**/        return (ARRAY *)NWordArrayCopy(nsuperof(array));        \
/**/    }                                                           \
/**/    int ARRAY##Count(ARRAY *array) {                            \
/**/        return NWordArrayCount(nsuperof(array));                \
/**/    }                                                           \
/**/    NIterator *ARRAY##Items(ARRAY *array) {                     \
/**/        return NWordArrayItems(nsuperof(array));                \
/**/    }                                                           \
/**/    void ARRAY##Push(ARRAY *array, TYPE item) {                 \
/**/        NWord elem = {0};                                       \
/**/        elem.MEMBER = item;                                     \
/**/        NWordArrayPush(nsuperof(array), elem);                  \
/**/    }                                                           \
/**/    void ARRAY##Pop(ARRAY *array) {                             \
/**/        NWordArrayPop(nsuperof(array));                         \
/**/    }                                                           \
/**/    void ARRAY##Insert(ARRAY *array, int index, TYPE item) {    \
/**/        NWord elem = {0};                                       \
/**/        elem.MEMBER = item;                                     \
/**/        NWordArrayInsert(nsuperof(array), index, elem);         \
/**/    }                                                           \
/**/    void ARRAY##Remove(ARRAY *array, int index) {               \
/**/        NWordArrayRemove(nsuperof(array), index);               \
/**/    }                                                           \
/**/    void ARRAY##Set(ARRAY *array, int index, TYPE item) {       \
/**/        NWord elem = {0};                                       \
/**/        elem.MEMBER = item;                                     \
/**/        NWordArraySet(nsuperof(array), index, elem);            \
/**/    }                                                           \
/**/    TYPE ARRAY##Get(ARRAY *array, int index) {                  \
/**/        NWord elem = NWordArrayGet(nsuperof(array), index);     \
/**/        return elem.MEMBER;                                     \
/**/    }

GEN_ARRAY(NArray   , NRef   , true , asPtr   )
GEN_ARRAY(NIntArray, int64_t, false, asInt64 )
GEN_ARRAY(NFltArray, double , false, asDouble)
