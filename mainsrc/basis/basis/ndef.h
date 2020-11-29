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

//supported types:
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
    typedef uint32_t char32_t;
    typedef uint16_t char16_t;
#endif

#define nstruct(n) typedef struct n n; struct n
#define nenum(  n) typedef int      n; enum

#define nisizeof(type) ((int)sizeof(type))

nenum(NType) {
    NTypeVoid   =  1,
    NTypeBool   =  2,
    NTypeInt8   =  3,
    NTypeInt16  =  4,
    NTypeInt32  =  5,
    NTypeInt64  =  6,
    NTypeUInt8  =  7,
    NTypeUInt16 =  8,
    NTypeUInt32 =  9,
    NTypeUInt64 = 10,
    NTypeFloat  = 11,
    NTypeDouble = 12,
    NTypePtr    = 13,
    NTypeStruct = 14,
};

//used as a flag for generating function meta data:
#if __cplusplus
    #define __nfunc(ret, name, params) extern "C" ret name params
#else
    #define __nfunc(ret, name, params) ret name params
#endif

#define nfunc(ret, name, params) __nfunc(ret, name, params)
