#pragma once

#include "basis.h"

nfunc(int, NStructsBegin, (void));
nfunc(int, NStructsEnd  , (void));
nfunc(int, NFindStruct  , (const char *name));

nfunc(const char *, NStructName , (int index));
nfunc(int         , NStructSuper, (int index));
nfunc(int         , NStructSize , (int index));
