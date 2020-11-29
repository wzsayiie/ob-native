#pragma once

//which compiler:
#if _MSC_VER
    #define N_COMPILER_CL    1
    #define N_COMPILER_CLANG 0
    #define N_COMPILER_GCC   0
#elif __clang__
    #define N_COMPILER_CL    0
    #define N_COMPILER_CLANG 1
    #define N_COMPILER_GCC   0
#elif __GNUC__
    #define N_COMPILER_CL    0
    #define N_COMPILER_CLANG 0
    #define N_COMPILER_GCC   1
#else
    #error "unknown compiler."
#endif

//how many bits of cpu word:
#if _WIN64 || (__LP64__ || __ILP64__ || __LLP64__)
    #define N_PTR_64 1
    #define N_PTR_32 0
#elif _WIN32 || (__LP32__ || __ILP32__)
    #define N_PTR_64 0
    #define N_PTR_32 1
#else
    #error "unknown cpu word."
#endif

//which of the following os platform:
//
// +-------+ +-------------+ +---------+
// |       | |     mac     | | windows |
// |android| | +---+ +---+ | | +-----+ |
// |       | | |ios| |osx| | | |win32| |
// |       | | +---+ +---+ | | +-----+ |
// +-------+ +-------------+ +---------+
//
#if __APPLE__
    #include <TargetConditionals.h>
#endif
#if ANDROID
    #define N_OS_ANDROID 1
    #define N_OS_IOS     0
    #define N_OS_OSX     0
    #define N_OS_WIN32   0
#elif TARGET_OS_IOS
    #define N_OS_ANDROID 0
    #define N_OS_IOS     1
    #define N_OS_OSX     0
    #define N_OS_WIN32   0
#elif TARGET_OS_OSX
    #define N_OS_ANDROID 0
    #define N_OS_IOS     0
    #define N_OS_OSX     1
    #define N_OS_WIN32   0
#elif _WIN32
    #define N_OS_ANDROID 0
    #define N_OS_IOS     0
    #define N_OS_OSX     0
    #define N_OS_WIN32   1
#else
    #error "unknown os platform."
#endif
