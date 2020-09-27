#include "cobj.h"
#include <stdlib.h>

defstruct(cobjreal) {
    int refn;
    cdeleter del;
};

cref newcobj(size_t size, cdeleter del) {
    if (size == 0) {
        return NULL;
    }

    cobjreal *obj = calloc(1, size);
    if (obj) {
        obj->refn = 1;
        obj->del = del;
    }
    return obj;
}

void _delcobj(cobj *obj) {
}

cref retain(cref ref) {
    if (!ref) {
        return NULL;
    }

    cobjreal *obj = ref;
    synchronize(obj) {
        obj->refn += 1;
    }
    return obj;
}

void release(cref ref) {
    if (!ref) {
        return;
    }

    cobjreal *obj = ref;
    synchronize(obj) {
        if (obj->refn-- == 1) {
            if (obj->del) {
                obj->del(obj);
            }
            free(obj);
        }
    }
}
