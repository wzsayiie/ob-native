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
//only 64 and 32 bits are supported.
#if _WIN64 || __LP64__
    #define N_PTR_64 1
    #define N_PTR_32 0
#else
    #define N_PTR_64 0
    #define N_PTR_32 1
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
    NTypeVoid  = 1,
    NTypeBool  = 2,
    NTypeInt8  = 3,
    NTypeInt16 = 4,
    NTypeInt32 = 5,
    NTypeInt64 = 6,
    NTypeFlt32 = 7,
    NTypeFlt64 = 8,
    NTypePtr   = 9,
};

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#if N_PTR_64
    typedef int64_t npbool;
    typedef int64_t npint;
    typedef double  npfloat;
#else
    typedef int32_t npbool;
    typedef int32_t npint;
    typedef float   npfloat;
#endif

#if !__cplusplus
    //"charxx_t" are not builtin types for c.
    typedef uint16_t char16_t;
    typedef uint32_t char32_t;
#endif

#define nisizeof(type) ((int)sizeof(type))

//used as a flag for generating function meta data:
#if __cplusplus
    #define __nfunc(ret, name, params) extern "C" ret name params
#else
    #define __nfunc(ret, name, params) ret name params
#endif

#ifndef nfunc
#define nfunc(ret, name, params) __nfunc(ret, name, params)
#endif

//mutex lock:
nfunc(void, NLock  , (int hash));
nfunc(void, NUnlock, (int hash));

#define nsynwith(n) \
/**/    for (int __N = (NLock((int)(npint)n), 1); __N; __N = (NUnlock((int)(npint)n), 0)) \
/**/    for (int __M = 1; __M; __M = 0)

#define nsyn() nsynwith(__LINE__)
