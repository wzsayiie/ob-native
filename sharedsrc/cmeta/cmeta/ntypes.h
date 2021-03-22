#pragma once

#include "nenviron.h"

nfunc(NType, NStructsBegin, (void));
nfunc(NType, NStructsEnd  , (void));
nfunc(NType, NFindType    , (const char *name));

nfunc(const char *, NTypeName, (NType type));

nfunc(int  , NTypeSize , (NType type));
nfunc(NType, NTypeSuper, (NType type));
