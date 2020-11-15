#pragma once

#include "ndef.h"

//create : allocate memory block and set its reference count to 1.
//retain : increment the reference count by 1.
//release: decrease the count by 1. if the count reduced to 0, delete the block.

nfunc(void *, NCreate )(int   bsize, /* void (*)(void *) */void *clear);
nfunc(void *, NRetain )(void *block);
nfunc(void  , NRelease)(void *block);
