#pragma once

#include "basis.h"

nfunc(int, NFindFunc)(const char *name);

nfunc(const char *, NFuncName      )(int funcID);
nfunc(      void *, NFuncAddress   )(int funcID);
nfunc(      NType , NFuncReturnType)(int funcID);
nfunc(      int   , NFuncParamCount)(int funcID);
nfunc(      NType , NFuncParamType )(int funcID, int paramIndex);
