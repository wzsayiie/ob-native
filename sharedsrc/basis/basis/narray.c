#include "narray.h"

static const int MAX_RESERVE_ITEM_NUM = 64;
static const int EVERY_ALLOC_ITEM_NUM = 16;

static void WordArrayStretch(NWordArray *array, int least) {
    int single   = array->conf.itemSize;
    int capacity = NMemorySize(array->items) / single;
    int vacancy  = capacity - array->count;

    if (vacancy < least) {
        int enough = array->count + least;
        if (enough % EVERY_ALLOC_ITEM_NUM > 0) {
            enough = (enough / EVERY_ALLOC_ITEM_NUM + 1) * EVERY_ALLOC_ITEM_NUM;
        }
        array->items = NReallocMemory(array->items, enough * single);
    }
}

static void WordArrayShrink(NWordArray *array) {
    int single   = array->conf.itemSize;
    int capacity = NMemorySize(array->items) / single;
    int vacancy  = capacity - array->count;

    if (vacancy > MAX_RESERVE_ITEM_NUM) {
        int enough = array->count + EVERY_ALLOC_ITEM_NUM;
        enough -= enough % EVERY_ALLOC_ITEM_NUM;
        array->items = NReallocMemory(array->items, enough * single);
    }
}

static void *WordArrayLocate(NWordArray *array, int index) {
    int offset = array->conf.itemSize * index;
    return (uint8_t *)array->items + offset;
}

static NWord WordArrayRead(NWordArray *array, int index) {
    NWord item = {0};
    void *ptr  = WordArrayLocate(array, index);
    NMoveMemory(&item, ptr, array->conf.itemSize);
    return item;
}

static void WordArrayWrite(NWordArray *array, int index, NWord item) {
    if (array->conf.itemRetain) {
        NRetain(item.asPtr);
    }

    void *ptr = WordArrayLocate(array, index);
    NMoveMemory(ptr, &item, array->conf.itemSize);
}

static void WordArrayErase(NWordArray *array, int index) {
    if (!array->conf.itemRetain) {
        return;
    }

    NWord item = {0};
    void *ptr  = WordArrayLocate(array, index);
    NMoveMemory(&item, ptr, array->conf.itemSize);
    NRelease(item.asPtr);
}

static void WordArrayMove(NWordArray *array, int index, int offset) {
    void *src = WordArrayLocate(array, index);
    void *dst = WordArrayLocate(array, index + offset);
    int   len = array->count - index;

    NMoveMemory(dst, src, len * array->conf.itemSize);
}

void _NWordArrayInit(NWordArray *array, NWordArrayConf *conf) {
    _NObjectInit(nsuperof(array));

    array->conf = *conf;
}

void _NWordArrayDeinit(NWordArray *array) {
    if (array->conf.itemRetain) {
        for (int n = 0; n < (array->count); ++n) {
            NWord item = WordArrayRead(array, n);
            NRelease(item.asPtr);
        }
    }
    NFreeMemory(array->items);
    
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
    WordArrayStretch(array, that->count);
    NMoveMemory(array->items, that->items, that->count * that->conf.itemSize);
    array->count = that->count;

    if (array->conf.itemRetain) {
        for (int n = 0; n < (array->count); ++n) {
            NWord item = WordArrayRead(array, n);
            NRetain(item.asPtr);
        }
    }

    return array;
}

int NWordArrayCount(NWordArray *array) {
    if (array) {
        return array->count;
    }
    return 0;
}

nstruct(NWordArrayIterator, {
    NIterator   super;
    NWordArray *array;
    int         ready;
});

static bool NWordArrayIteratorNext(NWordArrayIterator *iterator) {
    return (iterator->ready) < (iterator->array->count);
}

static void *NWordArrayIteratorGet(NWordArrayIterator *iterator) {
    uint8_t  *begin = iterator->array->items;
    ptrdiff_t every = iterator->array->conf.itemSize;

    return begin + every * (iterator->ready)++;
}

NIterator *NWordArrayItems(NWordArray *array) {
    if (!array) {
        return NStoreIterator(NULL, 0);
    }

    NWordArrayIterator iterator = {0};

    iterator.super.next = (NIteratorNextFunc)NWordArrayIteratorNext;
    iterator.super.get  = (NIteratorGetFunc )NWordArrayIteratorGet ;
    iterator.array = array;
    iterator.ready = 0;

    return NStoreIterator(&iterator, nsizeof(iterator));
}

void NWordArrayPush(NWordArray *array, NWord item) {
    if (!array) {
        return;
    }

    WordArrayStretch(array, 1);
    WordArrayWrite(array, array->count, item);
    array->count += 1;
}

void NWordArrayPop(NWordArray *array) {
    if (!array) {
        return;
    }
    if (array->count == 0) {
        return;
    }

    array->count -= 1;
    WordArrayErase(array, array->count);
    WordArrayShrink(array);
}

void NWordArrayInsert(NWordArray *array, int index, NWord item) {
    if (!array) {
        return;
    }
    if (index < 0 || (array->count) < index /* legal when index == count */) {
        return;
    }

    WordArrayStretch(array, 1);

    WordArrayMove(array, index, 1);
    WordArrayWrite(array, index, item);
    array->count += 1;
}

void NWordArrayRemove(NWordArray *array, int index) {
    if (!array) {
        return;
    }
    if (index < 0 || (array->count) <= index) {
        return;
    }

    WordArrayErase(array, index);
    WordArrayMove(array, index + 1, -1);
    array->count -= 1;

    WordArrayShrink(array);
}

void NWordArraySet(NWordArray *array, int index, NWord item) {
    if (array) {
        if (0 <= index && index < (array->count)) {
            WordArrayErase(array, index);
            WordArrayWrite(array, index, item);
        }
    }
}

NWord NWordArrayGet(NWordArray *array, int index) {
    if (array) {
        if (0 <= index && index < (array->count)) {
            return WordArrayRead(array, index);
        }
    }
    NWord word = {0};
    return word;
}

#define GEN_ARRAY(ARRAY, I_TYPE, I_RETAIN, MEMBER)                  \
/**/                                                                \
/**/    ARRAY *ARRAY##Create(void) {                                \
/**/        NWordArrayConf conf = {0};                              \
/**/                                                                \
/**/        conf.itemRetain = I_RETAIN;                             \
/**/        conf.itemSize = nsizeof(I_TYPE);                        \
/**/                                                                \
/**/        return (ARRAY *)NWordArrayCreate(&conf);                \
/**/    }                                                           \
/**/    ARRAY *ARRAY##Copy(ARRAY *array) {                          \
/**/        return (ARRAY *)NWordArrayCopy((NWordArray *)array);    \
/**/    }                                                           \
/**/    int ARRAY##Count(ARRAY *array) {                            \
/**/        return NWordArrayCount((NWordArray *)array);            \
/**/    }                                                           \
/**/    NIterator *ARRAY##Items(ARRAY *array) {                     \
/**/        return NWordArrayItems((NWordArray *)array);            \
/**/    }                                                           \
/**/    void ARRAY##Push(ARRAY *array, I_TYPE item) {               \
/**/        NWord word = {0};                                       \
/**/        word.MEMBER = item;                                     \
/**/        NWordArrayPush((NWordArray *)array, word);              \
/**/    }                                                           \
/**/    void ARRAY##Pop(ARRAY *array) {                             \
/**/        NWordArrayPop((NWordArray *)array);                     \
/**/    }                                                           \
/**/    void ARRAY##Insert(ARRAY *array, int index, I_TYPE item) {  \
/**/        NWord word = {0};                                       \
/**/        word.MEMBER = item;                                     \
/**/        NWordArrayInsert((NWordArray *)array, index, word);     \
/**/    }                                                           \
/**/    void ARRAY##Remove(ARRAY *array, int index) {               \
/**/        NWordArrayRemove((NWordArray *)array, index);           \
/**/    }                                                           \
/**/    void ARRAY##Set(ARRAY *array, int index, I_TYPE item) {     \
/**/        NWord word = {0};                                       \
/**/        word.MEMBER = item;                                     \
/**/        NWordArraySet((NWordArray *)array, index, word);        \
/**/    }                                                           \
/**/    I_TYPE ARRAY##Get(ARRAY *array, int index) {                \
/**/        return NWordArrayGet((NWordArray *)array, index).MEMBER;\
/**/    }

GEN_ARRAY(NArray   , NRef   , true , asPtr   )
GEN_ARRAY(NIntArray, int64_t, false, asInt64 )
GEN_ARRAY(NFltArray, double , false, asDouble)
