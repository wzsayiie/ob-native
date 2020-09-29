#pragma once

#include "macro.h"

//the reference to a "cobj" and the object's deleter.
typedef void *cref;
typedef void (*cdeleter)(cref ref);

//allocation argument.
defstruct(narg) {
    size_t   size;
    cdeleter delr;
};
narg mknarg(size_t size, cdeleter delr);

//the base class of non-pod (plain old data) type.
defstruct(cobj) {
    int      refn;
    cdeleter delr;
};

cref  newcobj(narg  arg);
void _delcobj(cobj *obj);

#define derives(cls) mknarg(sizeof(cls), (cdeleter)_del##cls)
#define superof(obj) &obj->super

cref retain (cref ref);
void release(cref ref);
