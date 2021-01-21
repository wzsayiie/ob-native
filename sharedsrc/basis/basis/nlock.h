#pragma once

#include "nenviron.h"

nfunc(void, NLock  , (int hash));
nfunc(void, NUnlock, (int hash));

#define nsynwith(n) \
/**/    for (int __N = (NLock((int)(intptr_t)n), 1); __N; __N = (NUnlock((int)(intptr_t)n), 0)) \
/**/    for (int __M = 1; __M; __M = 0)

#define nsynonce()          \
/**/    static int __T = 0; \
/**/    nsynwith(&__T)      \
/**/        if (__T == 0)   \
/**/            if (++__T)

#define nsyn() nsynwith(__LINE__)
