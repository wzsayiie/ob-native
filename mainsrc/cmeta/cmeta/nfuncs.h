#pragma once

#include "basis.h"

nfunc(int, NFuncsIndexBegin, (void));
nfunc(int, NFuncsIndexEnd  , (void));

nfunc(int, NFindFunc, (const char *name));

nfunc(const char *, NFuncName      , (int fIndex));
nfunc(      void *, NFuncAddress   , (int fIndex));
nfunc(      NType , NFuncReturnType, (int fIndex));
nfunc(      int   , NFuncParamCount, (int fIndex));
nfunc(      NType , NFuncParamType , (int fIndex, int pIndex));
