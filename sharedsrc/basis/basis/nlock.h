#pragma once

#include "nenviron.h"

nfunc(void, NLock  , (int hash));
nfunc(void, NUnlock, (int hash));

#define nsynwith(n) \
/**/    for (int _a = (NLock((int)(intptr_t)n), 1); _a; _a = (NUnlock((int)(intptr_t)n), 0)) \
/**/    for (int _b = 1; _b; _b = 0)

#define nsynonce()          \
/**/    static int _t = 0;  \
/**/    nsynwith(&_t)       \
/**/        if (_t == 0)    \
/**/            if (++_t)

#define nsyn() nsynwith(__LINE__)
