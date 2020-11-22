#pragma once

#include "ndef.h"

//NCreate : allocate a pod object and set its reference count to 1.
//NRetain : increment the reference count by 1.
//NRelease: decrease the count by 1. if the count reduced to 0, delete the object.

nfunc(void *, NCreate , (int   length, void *clear));
nfunc(void *, NRetain , (void *object));
nfunc(void  , NRelease, (void *object));
