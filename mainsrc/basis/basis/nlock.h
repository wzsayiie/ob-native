#pragma once

#include "ntypes.h"

nfunc(void, NLock  , (int hash));
nfunc(void, NUnlock, (int hash));

#define nsynwith(n) \
/**/    for (int __N = (NLock((int)(npint)n), 1); __N; __N = (NUnlock((int)(npint)n), 0)) \
/**/    for (int __M = 1; __M; __M = 0)

#define nsyn() nsynwith(__LINE__)
