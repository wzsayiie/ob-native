#pragma once

#include "nenviron.h"

nclink void _NInitLocks(void);

nfunc(void, NLock  , (int64_t hash));
nfunc(void, NUnlock, (int64_t hash));

#define for_lock(...)                                              \
/**/    for (int                                                   \
/**/        _a = (NLock  ((int64_t)(__LINE__, ##__VA_ARGS__)), 1); \
/**/        _a;                                                    \
/**/        _a = (NUnlock((int64_t)(__LINE__, ##__VA_ARGS__)), 0)  \
/**/    )                                                          \
/**/    for (int _b = 1; _b; _b = 0)

#define for_once()         \
/**/    static int _t = 0; \
/**/    for_lock(&_t)      \
/**/    if (_t == 0)       \
/**/    if (++_t)
