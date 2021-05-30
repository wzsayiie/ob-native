#pragma once

#include "nenviron.h"

nclink void NPrintInfo (const char *format, ...);
nclink void NPrintError(const char *format, ...);

nfunc(void, NPutInfo , (const char *string));
nfunc(void, NPutError, (const char *string));

//internal error ouput.
nclink void _NError(const char *format, ...);
