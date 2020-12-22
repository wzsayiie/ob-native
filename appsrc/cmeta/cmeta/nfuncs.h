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

//the memory layout of "__NWord" is same with "NWord" 's.
#ifndef __NWord
#define __NWord int64_t
#endif

#ifndef NFUNC_MAX_ARG_NUM
#define NFUNC_MAX_ARG_NUM 4
#endif

nfunc(__NWord, NCallFunc, (int fPos, int argc, NType *types, __NWord *words));
