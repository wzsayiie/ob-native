#include "cobj.h"
#include <stdlib.h>

narg mknarg(size_t size, cdeleter del) {
    narg arg;
    arg.size = size;
    arg.delr = del;
    return arg;
}

cref newcobj(narg arg) {
    cobj *obj = calloc(1, arg.size);
    if (obj) {
        obj->refn = 1;
        obj->delr = arg.delr;
    }
    return obj;
}

void _delcobj(cobj *obj) {
}

cref retain(cref ref) {
    if (!ref) {
        return NULL;
    }

    cobj *obj = ref;
    synchronize(obj) {
        obj->refn += 1;
    }
    return obj;
}

void release(cref ref) {
    if (!ref) {
        return;
    }

    cobj *obj = ref;
    synchronize(obj) {
        if (obj->refn-- == 1) {
            if (obj->delr) {
                obj->delr(obj);
            }
            free(obj);
        }
    }
}
