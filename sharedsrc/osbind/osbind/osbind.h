#pragma once

#include "basis.h"

#if NOS_ANDROID
    #include "njnibind.h"
#elif NOS_IOS || NOS_OSX
    #include "NOBJCBind.h"
#elif NOS_WIN32
    #include "NWin32Bind.h"
#endif
