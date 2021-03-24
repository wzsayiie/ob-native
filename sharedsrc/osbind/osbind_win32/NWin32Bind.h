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

nfunc(void, NWINApiPrepare, (void));
nfunc(void, NWINApiPushPtr, (const void *arg));
nfunc(void, NWINApiPushInt, (int64_t     arg));

nfunc(void   , NWINApiCallVoid, (NWINApi *api));
nfunc(void  *, NWINApiCallPtr , (NWINApi *api));
nfunc(int64_t, NWINApiCallInt , (NWINApi *api));
