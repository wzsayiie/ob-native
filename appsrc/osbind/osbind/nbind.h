#pragma once

#include "basis.h"

#if N_OS_ANDROID
    #include "nbind_android.h"
#endif

#if N_OS_IOS || N_OS_OSX
    #include "nbind_nmac.h"
#endif
