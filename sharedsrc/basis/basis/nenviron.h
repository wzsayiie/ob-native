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
// +---------------------------+ +---------+
// |           posix           | |         |
// | +-------+ +-------------+ | | windows |
// | |       | |     mac     | | | +-----+ |
// | |android| | +---+ +---+ | | | |     | |
// | |       | | |ios| |osx| | | | |win32| |
// | |       | | +---+ +---+ | | | |     | |
// | +-------+ +-------------+ | | +-----+ |
// +---------------------------+ +---------+
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

//types supported:
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#if !__cplusplus
    //"charxx_t" are not builtin types for c.
    typedef uint32_t char32_t;
    typedef uint16_t char16_t;
#endif

#define nenum(name) typedef int name; enum

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

  #if __ILP64__
    NTypeInt  = NTypeInt64 ,
    NTypeUInt = NTypeUInt64,
  #else
    NTypeInt  = NTypeInt32 ,
    NTypeUInt = NTypeUInt32,
  #endif
};

//type modifiers:
#define nisizeof(type) ((int)sizeof(type))

#if N_COMPILER_CL
    #define nthreadlocal __declspec(thread)
#else
    #define nthreadlocal _Thread_local
#endif

#if __cplusplus
    #define nclink extern "C"
#else
    #define nclink
#endif

//the flags for generating meta data:
#define __nclass(name) typedef struct name name; struct name
#define   nclass(name) __nclass(name)

#define __nfunc(ret, name, params) nclink  ret  name  params
#define   nfunc(ret, name, params) __nfunc(ret, name, params)

#define __nstruct(name) typedef struct name name; struct name
#define   nstruct(name) __nstruct(name)
