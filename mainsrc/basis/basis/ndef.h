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

//which os:
#if __APPLE__
    #include <TargetConditionals.h>
#endif
#if ANDROID
    #define N_OS_ANDROID 1
    #define N_OS_IOS     0
    #define N_OS_MACOS   0
    #define N_OS_WINDOWS 0
#elif TARGET_OS_IOS
    #define N_OS_ANDROID 0
    #define N_OS_IOS     1
    #define N_OS_MACOS   0
    #define N_OS_WINDOWS 0
#elif TARGET_OS_OSX
    #define N_OS_ANDROID 0
    #define N_OS_IOS     0
    #define N_OS_MACOS   1
    #define N_OS_WINDOWS 0
#elif _WIN32
    #define N_OS_ANDROID 0
    #define N_OS_IOS     0
    #define N_OS_MACOS   0
    #define N_OS_WINDOWS 1
#else
    #error "unknown os."
#endif

//supported types:
#define nstruct(n) typedef struct n n; struct n
#define nenum(  n) typedef int      n; enum

nenum(NType) {
    NTypeBool  = 1,
    NTypeInt8  = 2,
    NTypeInt16 = 3,
    NTypeInt32 = 4,
    NTypeInt64 = 5,
    NTypeFlt32 = 6,
    NTypeFlt64 = 7,
    NTypePtr   = 8,
};

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#if !__cplusplus
    //"charxx_t" are not builtin types for c.
    typedef unsigned short char16_t;
    typedef unsigned int   char32_t;
#endif

#define nisizeof(type) ((int)sizeof(type))

//used as a flag for generating function meta data:
#if __cplusplus
    #define __nfunc(retv, name) extern "C" retv name
#else
    #define __nfunc(retv, name) retv name
#endif

#define nfunc __nfunc

//mutex lock:
nfunc(void, NLock  )(int hash);
nfunc(void, NUnlock)(int hash);

#define nsynwith(n)                               \
/**/    for (bool                                 \
/**/        __N = (NLock  ((int)(intptr_t)n), 1); \
/**/        __N;                                  \
/**/        __N = (NUnlock((int)(intptr_t)n), 0)  \
/**/    )

#define nsyn() nsynwith(__LINE__)
