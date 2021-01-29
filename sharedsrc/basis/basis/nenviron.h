#pragma once

//which compiler.
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

//how many bits of cpu word.
#if _WIN64 || (__LP64__ || __ILP64__ || __LLP64__)
    #define N_PTR_64 1
    #define N_PTR_32 0
#elif _WIN32 || (__LP32__ || __ILP32__)
    #define N_PTR_64 0
    #define N_PTR_32 1
#else
    #error "unknown cpu word."
#endif

//which of the following os platform.
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

    NTypeStruct = 1, //unknown struct.
    NTypePtr    = 2, //unknown ptr.

    NTypeVoid   =  3,
    NTypeBool   =  4,
    NTypeChar8  =  5,
    NTypeChar16 =  6,
    NTypeChar32 =  7,
    NTypeInt8   =  8,
    NTypeInt16  =  9,
    NTypeInt32  = 10,
    NTypeInt64  = 11,
    NTypeUInt8  = 12,
    NTypeUInt16 = 13,
    NTypeUInt32 = 14,
    NTypeUInt64 = 15,
    NTypeFloat  = 16,
    NTypeDouble = 17,

    NTypeVoidPtr   = 18,
    NTypeBoolPtr   = 19,
    NTypeChar8Ptr  = 20,
    NTypeChar16Ptr = 21,
    NTypeChar32Ptr = 22,
    NTypeInt8Ptr   = 23,
    NTypeInt16Ptr  = 24,
    NTypeInt32Ptr  = 25,
    NTypeInt64Ptr  = 26,
    NTypeUInt8Ptr  = 27,
    NTypeUInt16Ptr = 28,
    NTypeUInt32Ptr = 29,
    NTypeUInt64Ptr = 30,
    NTypeFloatPtr  = 31,
    NTypeDoublePtr = 32,

    NTypeCustomPtr = 33,

  #if N_PTR_64
    NTypeInt     = NTypeInt64    ,
    NTypeUInt    = NTypeUInt64   ,
    NTypeIntPtr  = NTypeInt64Ptr ,
    NTypeUIntPtr = NTypeUInt64Ptr,
  #else
    NTypeInt     = NTypeInt32    ,
    NTypeUInt    = NTypeUInt32   ,
    NTypeIntPtr  = NTypeInt32Ptr ,
    NTypeUIntPtr = NTypeUInt32Ptr,
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

#define __nstruct(name, ...) typedef struct name name; struct name __VA_ARGS__
#define   nstruct(name, ...) __nstruct(name, __VA_ARGS__)

#define __nclass(name, super, ...) typedef struct name name; struct name __VA_ARGS__
#define   nclass(name, super, ...) __nclass(name, super, __VA_ARGS__)

#define __nfunc(ret, name, params) nclink  ret  name  params
#define   nfunc(ret, name, params) __nfunc(ret, name, params)
