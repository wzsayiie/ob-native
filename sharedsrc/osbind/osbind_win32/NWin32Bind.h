#pragma once

#include "basis.h"

nstruct(NWINLib, {int unused;});
nstruct(NWINApi, {int unused;});

nfunc(NWINLib *, NWINLoadLibrary   , (NString *dllPath));
nfunc(void     , NWINFreeLibrary   , (NWINLib *library));
nfunc(NWINApi *, NWINGetProcAddress, (NWINLib *library, const char *name));

//NOTE:
//there is no safe type checking when win32 apis are dynamically called.
//if the user doesn't set correct argument and return value types,
//the stack will be unbalanced, leading to unknowable consequences.

//due to the historical reason(cpus before 80486 have no built-in floating point unit),
//win32 apis doesn't use floating point types as function arguments or return values.

nfunc(void, NWINCallerReset  , (void));
nfunc(void, NWINCallerPushPtr, (const void *arg));
nfunc(void, NWINCallerPushInt, (int64_t     arg));

nfunc(void   , NWINCallVoid, (NWINApi *api));
nfunc(void  *, NWINCallPtr , (NWINApi *api));
nfunc(int64_t, NWINCallInt , (NWINApi *api));
