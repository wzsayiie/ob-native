#include "binlist.h"

static const int EACH_ALLOC_NUM = 64;

void blinit(binlist *list, cmpr cmp) {
    mzero(list, szof(*list));
    list->cmp = cmp;
}

void bldeinit(binlist *list) {
    mfree(list->dat);
    mzero(list, szof(*list));
}

static int search(binlist *list, word key, bool *hit) {
    //binary search here:
    int bgn = 0;
    int end = list->cnt;
    int idx = (bgn + end) / 2;

    while (bgn < end) {
        int res = list->cmp(key, list->dat[idx].key);
        if (res == 0) {
            //at least one same key here.
            *hit = true;
            return idx;
        }

        if (res > 0) {
            bgn = idx + 1;
        } else {
            end = idx;
        }

        idx = (bgn + end) / 2;
    }

    //suitable insertion position.
    return idx;
}

static void move(binlist *list, int bgn, int offset) {
    blitem *dst = list->dat + bgn + offset;
    blitem *src = list->dat + bgn;
    int     cnt = list->cnt - bgn;

    mmove(dst, src, cnt * szof(blitem));
}

int blinsert(binlist *list, word key, word val) {
    bool hit = false;
    int  idx = search(list, key, &hit);

    //allocate slots if needed.
    if (list->cnt == list->eom) {
        list->eom += EACH_ALLOC_NUM;
        list->dat = mrealloc(list->dat, list->eom, szof(blitem));
    }
    //insert the new item.
    if (idx < (list->cnt)) {
        move(list, idx, 1);
    }
    list->dat[idx].key = key;
    list->dat[idx].val = val;
    list->cnt += 1;

    return idx;
}

word blremove(binlist *list, word key) {
    bool hit = false;
    int  idx = search(list, key, &hit);

    if (hit) {
        return blcover(list, idx);
    } else {
        return iw(0);
    }
}

word blcover(binlist *list, int idx) {
    word item = bloffset(list, idx);
    move(list, idx + 1, -1);
    list->cnt -= 1;

    //free excess slots.
    int slot = list->eom - list->cnt;
    if (slot > EACH_ALLOC_NUM) {
        list->eom -= EACH_ALLOC_NUM;
        list->dat = mrealloc(list->dat, list->eom, szof(blitem));
    }

    return item;
}

int bladdup(binlist *list, word key) {
    int cnt = 0;
    for (int i = 0; i < list->cnt; ++i) {
        if (list->cmp(key, list->dat[i].key) == 0) {
            cnt += 1;
        }
    }
    return cnt;
}

int blcount(binlist *list) {
    return list->cnt;
}

bool blexist(binlist *list, word key) {
    return blindex(list, key) != -1;
}

word blvalue(binlist *list, word key) {
    bool hit = false;
    int  idx = search(list, key, &hit);

    if (hit) {
        return bloffset(list, idx);
    } else {
        return iw(0);
    }
}

int blindex(binlist *list, word key) {
    bool hit = false;
    int  idx = search(list, key, &hit);

    if (hit) {
        return idx;
    } else {
        return -1;
    }
}

word bloffset(binlist *list, int idx) {
    return list->dat[idx].val;
}
