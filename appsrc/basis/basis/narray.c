#include "narray.h"

nclass(__NArray) {
    int    type  ;
    NWord *items ;
    int    volume;
    int    count ;
};

static const int MAX_RESERVE_ITEM_NUM = 64;
static const int EVERY_ALLOC_ITEM_NUM = 16;

static void __NArrayTryStretch(__NArray *array, int least) {
    int vacancy = array->volume - array->count;
    if (vacancy >= least) {
        return;
    }

    int volume = array->count + least;
    if (volume % EVERY_ALLOC_ITEM_NUM > 0) {
        volume = (volume / EVERY_ALLOC_ITEM_NUM + 1) * EVERY_ALLOC_ITEM_NUM;
    }
    array->items  = NRealloc(array->items, volume * nisizeof(NWord));
    array->volume = volume;
}

static void __NArrayTryShrink(__NArray *array) {
    int vacancy = array->volume - array->count;
    if (vacancy <= MAX_RESERVE_ITEM_NUM) {
        return;
    }

    int volume = array->count + EVERY_ALLOC_ITEM_NUM;
    volume -= volume % EVERY_ALLOC_ITEM_NUM;

    array->items  = NRealloc(array->items, volume * nisizeof(NWord));
    array->volume = volume;
}

static bool __IsObjectArray(__NArray *array) {
    return array->type == __NArrayItemTypeObject;
}

static void __NArrayClear(__NArray *array) {
    if (__IsObjectArray(array)) {
        NWord *cur = array->items;
        NWord *end = array->items + array->count;
        for (; cur < end; ++cur) {
            NRelease(cur->asPtr);
        }
    }
    NFree(array->items);
}

__NArray *__NArrayCreate(__NArrayItemType type) {
    __NArray *self = NCreate(nisizeof(__NArray), __NArrayClear);
    self->type = type;
    return self;
}

__NArray *__NArrayCopy(__NArray *that) {
    if (!that) {
        return NULL;
    }

    __NArray *copy = __NArrayCreate(that->type);
    __NArrayTryStretch(copy, that->count);
    NMoveMemory(copy->items, that->items, that->count * nisizeof(NWord));
    copy->count = that->count;

    if (__IsObjectArray(copy)) {
        NWord *cur = copy->items;
        NWord *end = copy->items + copy->count;
        for (; cur < end; ++cur) {
            NRetain(cur->asPtr);
        }
    }

    return copy;
}

int __NArrayCount(__NArray *self) {
    if (self) {
        return self->count;
    }
    return 0;
}

void __NArrayPush(__NArray *self, NWord item) {
    if (!self) {
        return;
    }

    __NArrayTryStretch(self, 1);
    self->count += 1;

    NWord *last = self->items + self->count - 1;
    if (__IsObjectArray(self)) {
        last->asPtr = NRetain(item.asPtr);
    } else {
        *last = item;
    }
}

void __NArrayPop(__NArray *self) {
    if (!self) {
        return;
    }
    if (self->count == 0) {
        return;
    }

    if (__IsObjectArray(self)) {
        NWord last = self->items[self->count - 1];
        NRelease(last.asPtr);
    }

    self->count -= 1;
    __NArrayTryShrink(self);
}

static bool __NArrayReadablePos(__NArray *a, int p) {return 0 <= p && p <  a->count;}
static bool __NArrayWritablePos(__NArray *a, int p) {return 0 <= p && p <= a->count;}

static void __NArrayMoveItems(__NArray *array, int pos, int offset) {
    void *src = array->items + pos;
    void *dst = array->items + pos + offset;
    int   len = array->count - pos;

    NMoveMemory(dst, src, len * nisizeof(NWord));
}

void __NArrayInsert(__NArray *self, int pos, NWord item) {
    if (!self) {
        return;
    }
    if (!__NArrayWritablePos(self, pos)) {
        return;
    }

    __NArrayTryStretch(self, 1);
    __NArrayMoveItems(self, pos, 1);
    self->count += 1;

    NWord *slot = self->items + pos;
    if (__IsObjectArray(self)) {
        slot->asPtr = NRetain(item.asPtr);
    } else {
        *slot = item;
    }
}

void __NArrayRemove(__NArray *self, int pos) {
    if (!self) {
        return;
    }
    if (!__NArrayReadablePos(self, pos)) {
        return;
    }

    if (__IsObjectArray(self)) {
        NRelease(self->items[pos].asPtr);
    }

    __NArrayMoveItems(self, pos + 1, -1);
    self->count -= 1;
    __NArrayTryShrink(self);
}

void __NArraySet(__NArray *self, int pos, NWord item) {
    if (!self) {
        return;
    }
    if (!__NArrayReadablePos(self, pos)) {
        return;
    }

    NWord *slot = self->items + pos;
    if (__IsObjectArray(self)) {
        NRelease(slot->asPtr);
        slot->asPtr = NRetain(item.asPtr);
    } else {
        *slot = item;
    }
}

NWord __NArrayGet(__NArray *self, int pos) {
    if (self) {
        if (__NArrayReadablePos(self, pos)) {
            return self->items[pos];
        }
    }
    NWord word = {0};
    return word;
}

#define GEN_ARRAY(ARRAY, TYPE, ITEM, MEMBER)                    \
/**/                                                            \
/**/    ARRAY *ARRAY##Create(void) {                            \
/**/        return (ARRAY *)__NArrayCreate(TYPE);               \
/**/    }                                                       \
/**/    ARRAY *ARRAY##Copy(ARRAY *array) {                      \
/**/        return (ARRAY *)__NArrayCopy((__NArray *)array);    \
/**/    }                                                       \
/**/    int ARRAY##Count(ARRAY *array) {                        \
/**/        return __NArrayCount((__NArray *)array);            \
/**/    }                                                       \
/**/    void ARRAY##Push(ARRAY *array, ITEM item) {             \
/**/        NWord word;                                         \
/**/        word.MEMBER = item;                                 \
/**/        __NArrayPush((__NArray *)array, word);              \
/**/    }                                                       \
/**/    void ARRAY##Pop(ARRAY *array) {                         \
/**/        __NArrayPop((__NArray *)array);                     \
/**/    }                                                       \
/**/    void ARRAY##Insert(ARRAY *array, int pos, ITEM item) {  \
/**/        NWord word;                                         \
/**/        word.MEMBER = item;                                 \
/**/        __NArrayInsert((__NArray *)array, pos, word);       \
/**/    }                                                       \
/**/    void ARRAY##Remove(ARRAY *array, int pos) {             \
/**/        __NArrayRemove((__NArray *)array, pos);             \
/**/    }                                                       \
/**/    void ARRAY##Set(ARRAY *array, int pos, ITEM item) {     \
/**/        NWord word;                                         \
/**/        word.MEMBER = item;                                 \
/**/        __NArraySet((__NArray *)array, pos, word);          \
/**/    }                                                       \
/**/    ITEM ARRAY##Get(ARRAY *array, int pos) {                \
/**/        return __NArrayGet((__NArray *)array, pos).MEMBER;  \
/**/    }

GEN_ARRAY(NArray   , __NArrayItemTypeObject, NObject *, asPtr   )
GEN_ARRAY(NIntArray, __NArrayItemTypePOD   , int64_t  , asInt64 )
GEN_ARRAY(NDblArray, __NArrayItemTypePOD   , double   , asDouble)
