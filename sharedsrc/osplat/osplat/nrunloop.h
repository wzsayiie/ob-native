#pragma once

#include "basis.h"

typedef void (*NTaskFunc)(int64_t param);

nfunc(void, NPostTaskOnMain, (int64_t param, NTaskFunc func));
