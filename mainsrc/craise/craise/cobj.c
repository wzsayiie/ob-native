#include "cobj.h"
#include <stdlib.h>

defstruct(cobjreal) {
    int   refn;
    cdtor dtor;
};

cref __cnew(size_t size, cctor ctor, cdtor dtor) {
    if (size == 0) {
        return NULL;
    }

    cobjreal *real = calloc(1, size);
    if (real) {
        real->refn = 1;
        real->dtor = dtor;
        if (ctor) {
            ctor(real);
        }
    }
    return real;
}

cref cretain(cref obj) {
    if (!obj) {
        return obj;
    }

    cobjreal *real = obj;
    synchronize(real) {
        real->refn += 1;
    }
    return real;
}

void crelease(cref obj) {
    if (!obj) {
        return;
    }

    cobjreal *real = obj;
    synchronize(real) {
        if (real->refn-- == 1) {
            if (real->dtor) {
                real->dtor(real);
            }
            free(real);
        }
    }
}
