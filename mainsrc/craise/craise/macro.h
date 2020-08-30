#pragma once

// +-------+ +-------------+ +---------+
// |       | |     mac     | | windows |
// |android| | +---+ +---+ | | +-----+ |
// |       | | |ios| |osx| | | |win32| |
// |       | | +---+ +---+ | | +-----+ |
// +-------+ +-------------+ +---------+
//
# ifdef __APPLE__
#   include <TargetConditionals.h>
# endif
# if ANDROID
#   define ON_ANDROID 1
#   define ON_IOS     0
#   define ON_WINDOWS 0
#   define ON_OSX     0
# elif TARGET_OS_IOS
#   define ON_ANDROID 0
#   define ON_IOS     1
#   define ON_WINDOWS 0
#   define ON_OSX     0
# elif _WIN32
#   define ON_ANDROID 0
#   define ON_IOS     0
#   define ON_WINDOWS 1
#   define ON_OSX     0
# elif TARGET_OS_OSX
#   define ON_ANDROID 0
#   define ON_IOS     0
#   define ON_WINDOWS 0
#   define ON_OSX     1
# else
#   error "unknown os"
# endif

#define _struct(name)\
    typedef struct name name;\
    struct name
