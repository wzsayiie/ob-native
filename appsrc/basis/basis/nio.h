#pragma once

#include "ntypes.h"

nclink void NPrintF(const char *format, ...);

nfunc(void, NPutS , (const char *string));
nfunc(void, NFlush, (void));
