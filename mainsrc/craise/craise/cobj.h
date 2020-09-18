#pragma once

#include "macro.h"

typedef void *cref;

defstruct(cobj) {
    void *__dat[4];
};

typedef void (*cctor)(cref obj);
typedef void (*cdtor)(cref obj);

#define cnew(type, ctor, dtor) __cnew(sizeof(type), ctor, dtor)

cref __cnew  (size_t size, cctor ctor, cdtor dtor);
cref cretain (cref obj);
void crelease(cref obj);
