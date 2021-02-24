#pragma once

#include "nenviron.h"

nfunc(int, NFuncsBegin, (void));
nfunc(int, NFuncsEnd  , (void));
nfunc(int, NFindFunc  , (const char *name));

nfunc(const char *, NFuncName, (int fIndex));

nfunc(void *, NFuncAddress    , (int fIndex));
nfunc(NType , NFuncRetType    , (int fIndex));
nfunc(bool  , NFuncRetRetained, (int fIndex));
nfunc(int   , NFuncArgCount   , (int fIndex));
nfunc(NType , NFuncArgType    , (int fIndex, int aIndex));

//the memory layout of "_NWord" is same with "NWord" 's.
#ifndef _NWord
#define _NWord int64_t
#endif

nfunc(void  , NCallerReset, (void));
nfunc(void  , NCallerPush , (NType argType, _NWord argWord));
nfunc(_NWord, NCallFunc   , (int   fIndex));
