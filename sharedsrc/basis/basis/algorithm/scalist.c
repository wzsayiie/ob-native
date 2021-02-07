#include "scalist.h"

static const int EACH_ALLOC_NUM = 64;

void slinit(scalist *list) {
    mzero(list, szof(*list));
}

void sldeinit(scalist *list) {
    mfree(list->dat);
    mzero(list, szof(*list));
}

static void move(scalist *list, int bgn, int offset) {
    word *dst = list->dat + bgn + offset;
    word *src = list->dat + bgn;
    int   cnt = list->cnt - bgn;

    mmove(dst, src, cnt * szof(word));
}

void slinsert(scalist *list, int idx, word itm) {
    //allocate slots if needed.
    if (list->cnt == list->eom) {
        list->eom += EACH_ALLOC_NUM;
        list->dat = mrealloc(list->dat, list->eom, szof(word));
    }

    if (idx < (list->cnt)) {
        move(list, idx, 1);
    }
    list->dat[idx] = itm;
    list->cnt += 1;
}

word slremove(scalist *list, int idx) {
    word itm = slget(list, idx);

    if (idx + 1 < (list->cnt)) {
        move(list, idx + 1, -1);
    }
    list->cnt -= 1;

    //free excess slots.
    int slot = list->eom - list->cnt;
    if (slot > EACH_ALLOC_NUM) {
        list->eom -= EACH_ALLOC_NUM;
        list->dat = mrealloc(list->dat, list->eom, szof(word));
    }

    return itm;
}

void slpush(scalist *list, word itm) {
    slinsert(list, list->cnt, itm);
}

word slpop(scalist *list) {
    return slremove(list, list->cnt - 1);
}

int slcount(scalist *list) {
    return list->cnt;
}

void slset(scalist *list, int idx, word itm) {
    list->dat[idx] = itm;
}

word slget(scalist *list, int idx) {
    return list->dat[idx];
}
