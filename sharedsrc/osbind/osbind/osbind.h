#pragma once

#if N_OS_ANDROID
    #include "njnibind.h"
#endif

#if N_OS_IOS || N_OS_OSX
    #include "NOBJCBind.h"
#endif
