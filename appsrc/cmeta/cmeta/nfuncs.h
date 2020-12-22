#pragma once

#include "basis.h"

nfunc(int, NFuncsBegin, (void));
nfunc(int, NFuncsEnd  , (void));
nfunc(int, NFindFunc  , (const char *name));

nfunc(const char *, NFuncName, (int fPos));

nfunc(void *, NFuncAddress    , (int fPos));
nfunc(NType , NFuncRetType    , (int fpos));
nfunc(bool  , NFuncRetRetained, (int fPos));
nfunc(int   , NFuncArgCount   , (int fPos));
nfunc(NType , NFuncArgType    , (int fPos, int aPos));

nfunc(int64_t, NCallFunc, (int fPos, int argc, NType *types, int64_t *words));
