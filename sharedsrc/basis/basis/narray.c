#include "narray.h"

nclass(NWordArray, NObject, {
    NWordArrayConf conf;
    void *items;
    int count;
});

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

static void WordArrayClear(NWordArray *array) {
    if (array->conf.itemRetain) {
        for (int n = 0; n < (array->count); ++n) {
            NWord item = WordArrayRead(array, n);
            NRelease(item.asPtr);
        }
    }
    NFreeMemory(array->items);
}

NWordArray *NWordArrayCreate(NWordArrayConf *conf) {
    NWordArray *self = NCreate(nisizeof(NWordArray), WordArrayClear);
    self->conf = *conf;
    return self;
}

NWordArray *NWordArrayCopy(NWordArray *that) {
    if (!that) {
        return NULL;
    }

    NWordArray *self = NWordArrayCreate(&that->conf);
    WordArrayStretch(self, that->count);
    NMoveMemory(self->items, that->items, that->count * that->conf.itemSize);
    self->count = that->count;

    if (self->conf.itemRetain) {
        for (int n = 0; n < (self->count); ++n) {
            NWord item = WordArrayRead(self, n);
            NRetain(item.asPtr);
        }
    }

    return self;
}

int NWordArrayCount(NWordArray *self) {
    if (self) {
        return self->count;
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

NIterator *NWordArrayItems(NWordArray *self) {
    if (!self) {
        return NStoreIterator(NULL, 0);
    }

    NWordArrayIterator iterator = {0};

    iterator.super.next = (NIteratorNextFunc)NWordArrayIteratorNext;
    iterator.super.get  = (NIteratorGetFunc )NWordArrayIteratorGet ;
    iterator.array = self;
    iterator.ready = 0;

    return NStoreIterator(&iterator, nisizeof(iterator));
}

void NWordArrayPush(NWordArray *self, NWord item) {
    if (!self) {
        return;
    }

    WordArrayStretch(self, 1);
    WordArrayWrite(self, self->count, item);
    self->count += 1;
}

void NWordArrayPop(NWordArray *self) {
    if (!self) {
        return;
    }
    if (self->count == 0) {
        return;
    }

    self->count -= 1;
    WordArrayErase(self, self->count);
    WordArrayShrink(self);
}

void NWordArrayInsert(NWordArray *self, int index, NWord item) {
    if (!self) {
        return;
    }
    if (index < 0 || (self->count) < index /* legal when index == count */) {
        return;
    }

    WordArrayStretch(self, 1);

    WordArrayMove(self, index, 1);
    WordArrayWrite(self, index, item);
    self->count += 1;
}

void NWordArrayRemove(NWordArray *self, int index) {
    if (!self) {
        return;
    }
    if (index < 0 || (self->count) <= index) {
        return;
    }

    WordArrayErase(self, index);
    WordArrayMove(self, index + 1, -1);
    self->count -= 1;

    WordArrayShrink(self);
}

void NWordArraySet(NWordArray *self, int index, NWord item) {
    if (self) {
        if (0 <= index && index < (self->count)) {
            WordArrayErase(self, index);
            WordArrayWrite(self, index, item);
        }
    }
}

NWord NWordArrayGet(NWordArray *self, int index) {
    if (self) {
        if (0 <= index && index < (self->count)) {
            return WordArrayRead(self, index);
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
/**/        conf.itemSize = nisizeof(I_TYPE);                       \
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

GEN_ARRAY(NArray   , NObject *, true , asPtr   )
GEN_ARRAY(NIntArray, int64_t  , false, asInt64 )
GEN_ARRAY(NFltArray, double   , false, asDouble)
