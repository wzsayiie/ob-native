#include "dearray.h"

void dearray_init(dearray *self, size_t itsz) {
    self->mptr = NULL;
    self->itsz = itsz;
    self->hnum = 0;
    self->inum = 0;
    self->tnum = 0;
}

void dearray_deinit(dearray *self) {
    free(self->mptr);
    memzero(self, sizeof(dearray));
}

void dearray_insert(dearray *self, int idx, void *item) {
}

void dearray_remove(dearray *self, int idx) {
}

void dearray_clear(dearray *self) {
    free(self->mptr);
    self->mptr = NULL;
    
    //NOTE: keep 'itsz' unchanged.
    
    self->hnum = 0;
    self->inum = 0;
    self->tnum = 0;
}

int dearray_len(dearray *self) {
    return self->inum;
}

void *dearray_at(dearray *self, int idx) {
    return NULL;
}
