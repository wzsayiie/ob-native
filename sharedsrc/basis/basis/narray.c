#include "narray.h"

nclass(NWordArray) {
    NWordArrayConf conf;
    void *items;
    int count;
};

static const int MAX_RESERVE_ITEM_NUM = 64;
static const int EVERY_ALLOC_ITEM_NUM = 16;

static void _NWordArrayStretch(NWordArray *array, int least) {
    int single   = array->conf.itemSize;
    int capacity = NMemorySize(array->items) / single;
    int vacancy  = capacity - array->count;

    if (vacancy < least) {
        int enough = array->count + least;
        if (enough % EVERY_ALLOC_ITEM_NUM > 0) {
            enough = (enough / EVERY_ALLOC_ITEM_NUM + 1) * EVERY_ALLOC_ITEM_NUM;
        }
        array->items = NRealloc(array->items, enough * single);
    }
}

static void _NWordArrayShrink(NWordArray *array) {
    int single   = array->conf.itemSize;
    int capacity = NMemorySize(array->items) / single;
    int vacancy  = capacity - array->count;

    if (vacancy > MAX_RESERVE_ITEM_NUM) {
        int enough = array->count + EVERY_ALLOC_ITEM_NUM;
        enough -= enough % EVERY_ALLOC_ITEM_NUM;
        array->items = NRealloc(array->items, enough * single);
    }
}

static void *_NWordArrayLocate(NWordArray *array, int index) {
    int offset = array->conf.itemSize * index;
    return (uint8_t *)array->items + offset;
}

static NWord _NWordArrayRead(NWordArray *array, int index) {
    NWord item = {0};
    void *ptr  = _NWordArrayLocate(array, index);
    NMoveMemory(&item, ptr, array->conf.itemSize);
    return item;
}

static void _NWordArrayWrite(NWordArray *array, int index, NWord item) {
    if (array->conf.itemRetain) {
        NRetain(item.asPtr);
    }

    void *ptr = _NWordArrayLocate(array, index);
    NMoveMemory(ptr, &item, array->conf.itemSize);
}

static void _NWordArrayErase(NWordArray *array, int index) {
    if (!array->conf.itemRetain) {
        return;
    }

    NWord item = {0};
    void *ptr  = _NWordArrayLocate(array, index);
    NMoveMemory(&item, ptr, array->conf.itemSize);
    NRelease(item.asPtr);
}

static void _NWordArrayMove(NWordArray *array, int index, int offset) {
    void *src = _NWordArrayLocate(array, index);
    void *dst = _NWordArrayLocate(array, index + offset);
    int   len = array->count - index;

    NMoveMemory(dst, src, len * array->conf.itemSize);
}

static void _NWordArrayClear(NWordArray *array) {
    if (array->conf.itemRetain) {
        for (int n = 0; n < (array->count); ++n) {
            NWord item = _NWordArrayRead(array, n);
            NRelease(item.asPtr);
        }
    }
    NFree(array->items);
}

NWordArray *NWordArrayCreate(NWordArrayConf *conf) {
    NWordArray *self = NCreate(nisizeof(NWordArray), _NWordArrayClear);
    self->conf = *conf;
    return self;
}

NWordArray *NWordArrayCopy(NWordArray *that) {
    if (!that) {
        return NULL;
    }

    NWordArray *self = NWordArrayCreate(&that->conf);
    _NWordArrayStretch(self, that->count);
    NMoveMemory(self->items, that->items, that->count * that->conf.itemSize);
    self->count = that->count;

    if (self->conf.itemRetain) {
        for (int n = 0; n < (self->count); ++n) {
            NWord item = _NWordArrayRead(self, n);
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

nstruct(NWordArrayIterator) {
    NIterator   super;
    NWordArray *array;
    int         ready;
};

static bool NWordArrayIteratorNext(NWordArrayIterator *iterator) {
    return (iterator->ready) < (iterator->array->count);
}

static void *NWordArrayIteratorGet(NWordArrayIterator *iterator) {
    uint8_t *begin = iterator->array->items;
    int      every = iterator->array->conf.itemSize;

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

    _NWordArrayStretch(self, 1);
    _NWordArrayWrite(self, self->count, item);
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
    _NWordArrayErase(self, self->count);
    _NWordArrayShrink(self);
}

void NWordArrayInsert(NWordArray *self, int index, NWord item) {
    if (!self) {
        return;
    }
    if (index < 0 || (self->count) < index /* legal when index == count */) {
        return;
    }

    _NWordArrayStretch(self, 1);

    _NWordArrayMove(self, index, 1);
    _NWordArrayWrite(self, index, item);
    self->count += 1;
}

void NWordArrayRemove(NWordArray *self, int index) {
    if (!self) {
        return;
    }
    if (index < 0 || (self->count) <= index) {
        return;
    }

    _NWordArrayErase(self, index);
    _NWordArrayMove(self, index + 1, -1);
    self->count -= 1;

    _NWordArrayShrink(self);
}

void NWordArraySet(NWordArray *self, int index, NWord item) {
    if (self) {
        if (0 <= index && index < (self->count)) {
            _NWordArrayErase(self, index);
            _NWordArrayWrite(self, index, item);
        }
    }
}

NWord NWordArrayGet(NWordArray *self, int index) {
    if (self) {
        if (0 <= index && index < (self->count)) {
            return _NWordArrayRead(self, index);
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
