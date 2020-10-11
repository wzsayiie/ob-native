#pragma once

#include "pod.h"

defstruct(dearray) {
    int8_t *mptr; //memory block begin.
    size_t  itsz; //item size.
    int     hnum; //the number of the reserved slots at head.
    int     inum; //the number of the items.
    int     tnum; //the number of the reserved slots at tail.
};

//if N items are stored currently,
//at most (N * DEARRAY_RESERVE_FACTOR) slots can be reserved additionally.
static const float DEARRAY_RESERVE_FACTOR = 0.2;

void dearray_init  (dearray *self, size_t itsz);
void dearray_deinit(dearray *self);

void  dearray_insert(dearray *self, int idx, void *item);
void  dearray_remove(dearray *self, int idx);
void  dearray_clear (dearray *self);
int   dearray_len   (dearray *self);
void *dearray_at    (dearray *self, int idx);
