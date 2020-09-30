#pragma once

#include "pod.h"

defstruct(dearray) {
    int8_t *mptr; //memory block begin.
    int8_t *mend; //memory block end.
    int8_t *iptr; //item begin.
    int8_t *iend; //item end.
    size_t  isin; //single item size.
};

void dearray_init  (dearray *self, size_t isin);
void dearray_deinit(dearray *self);

void  dearray_insert(dearray *self, int idx, void *item);
void  dearray_remove(dearray *self, int idx);
void  dearray_clear (dearray *self);
int   dearray_len   (dearray *self);
void *dearray_at    (dearray *self, int idx);
