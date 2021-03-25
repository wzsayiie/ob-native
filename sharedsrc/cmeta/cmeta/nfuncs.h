#pragma once

#include "nenviron.h"

nfunc(int, NFuncsBegin, (void));
nfunc(int, NFuncsEnd  , (void));
nfunc(int, NFindFunc  , (const char *name));

nfunc(const char *, NFuncName, (int index));

nfunc(void *, NFuncAddress  , (int index));
nfunc(NType , NFuncRetType  , (int index));
nfunc(bool  , NFuncRetRetain, (int index));
nfunc(int   , NFuncArgCount , (int index));
nfunc(NType , NFuncArgType  , (int index, int argIndex));

//the memory layout of "_Word" is same with "NWord" 's.
typedef int64_t _Word;

nfunc(void , NFuncPrepare, (void));
nfunc(void , NFuncPushArg, (NType type, _Word word));
nfunc(_Word, NFuncCall   , (int index));
