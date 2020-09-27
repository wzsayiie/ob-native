#pragma once

#include "pod.h"

defstruct(dearray) {
    void  *mptr;  //memory block begin.
    void  *mend;  //memory block end (the position unavailable).
    void  *iptr;  //item begin.
    size_t inum;  //item number.
    size_t isize; //item size (NOTE: user assigning).
};

void dearray_init (dearray *array);
void dearray_clear(dearray *array);

void  dearray_add(dearray *array, void *item);
void  dearray_rm (dearray *array, int idx);
void *dearray_at (dearray *array, int idx);
