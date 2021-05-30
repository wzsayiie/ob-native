#pragma once

#include "basis.h"

typedef void (*NThreadFunc)(int64_t param);

nfunc(void, NRunThread  , (int64_t param, NThreadFunc func));
nfunc(void, NThreadSleep, (float seconds));
