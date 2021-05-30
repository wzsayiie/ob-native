#include "narray.h"

static void ArrayDestroy(NArray *array) {
    scalist *list = &array->list;
    for (int i = 0; i < slcount(list); ++i) {
        word item = slget(list, i);
        NRelease(item.asptr);
    }
    sldeinit(list);
}

NArray *NArrayCreate(void) {
    NArray *array = NCreate(NArray, ArrayDestroy);
    slinit(&array->list);
    return array;
}

NArray *NArrayCopy(NArray *that) {
    if (!that) {
        return NULL;
    }

    NArray *array = NArrayCreate();

    scalist *tlist = &that ->list;
    scalist *alist = &array->list;
    for (int i = 0; i < slcount(tlist); ++i) {
        word item = slget(tlist, i);
        NRetain(item.asptr);
        slpush(alist, item);
    }

    return array;
}

int NArrayCount(NArray *array) {
    if (array) {
        return slcount(&array->list);
    }
    return 0;
}

void NArrayPush(NArray *array, NObject *item) {
    if (array) {
        NRetain(item);
        slpush(&array->list, pw(item));
    }
}

void NArrayPop(NArray *array) {
    if (!array) {
        return;
    }
    
    scalist *list = &array->list;
    if (slcount(list) == 0) {
        return;
    }
    
    word item = slpop(list);
    NRelease(item.asptr);
}

void NArrayInsert(NArray *array, int index, NObject *item) {
    if (!array) {
        return;
    }
    
    scalist *list = &array->list;
    if (index < 0 || slcount(list) <= index) {
        return;
    }

    NRetain(item);
    slinsert(list, index, pw(item));
}

void NArrayRemove(NArray *array, int index) {
    if (!array) {
        return;
    }
    
    scalist *list = &array->list;
    if (index < 0 || slcount(list) <= index) {
        return;
    }

    word item = slremove(list, index);
    NRelease(item.asptr);
}

void NArraySet(NArray *array, int index, NObject *item) {
    if (!array) {
        return;
    }
    
    scalist *list = &array->list;
    if (index < 0 || slcount(list) <= index) {
        return;
    }
    
    word old = slget(list, index);
    NRelease(old.asptr);
    
    slset(list, index, pw(item));
    NRetain(item);
}

NObject *NWordArrayGet(NArray *array, int index) {
    if (!array) {
        return NULL;
    }
    
    scalist *list = &array->list;
    if (index < 0 || slcount(list) <= index) {
        return NULL;
    }
    
    word item = slget(list, index);
    return item.asptr;
}
