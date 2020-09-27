#pragma once

#include "macro.h"

//the reference to a "cobj" or its subclass instance.
typedef void *cref;

//deleter for a "cobj".
typedef void (*cdeleter)(cref ref);

defstruct(cobj) {
    void *__dat[4];
};

cref  newcobj(size_t size, cdeleter del);
void _delcobj(cobj *obj);

cref retain (cref ref);
void release(cref ref);
