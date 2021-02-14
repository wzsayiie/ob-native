#pragma once

#if NOS_ANDROID
    #include "njnibind.h"
#endif

#if NOS_IOS || NOS_OSX
    #include "NOBJCBind.h"
#endif
