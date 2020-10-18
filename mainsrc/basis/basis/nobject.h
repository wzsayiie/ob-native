#pragma once

#include "ndef.h"

//create : allocate object and set its reference count to 1.
//retain : increment the reference count by 1.
//release: decrease the count by 1. if the count reduced to 0, delete the object.

void *NCreate(int size, void *deinit);

void *NRetain (void *object);
void  NRelease(void *object);
