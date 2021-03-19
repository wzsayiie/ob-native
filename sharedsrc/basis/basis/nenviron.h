#pragma once

//which compiler.
#if _MSC_VER
    #define NCPL_CL    1
    #define NCPL_CLANG 0
    #define NCPL_GCC   0
#elif __clang__
    #define NCPL_CL    0
    #define NCPL_CLANG 1
    #define NCPL_GCC   0
#elif __GNUC__
    #define NCPL_CL    0
    #define NCPL_CLANG 0
    #define NCPL_GCC   1
#else
    #error "unknown compiler."
#endif

//how many bits of cpu word.
#if _WIN64 || (__LP64__ || __ILP64__ || __LLP64__)
    #define NPTR_64 1
    #define NPTR_32 0
#elif _WIN32 || (__LP32__ || __ILP32__)
    #define NPTR_64 0
    #define NPTR_32 1
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
    #define NOS_ANDROID 1
    #define NOS_IOS     0
    #define NOS_OSX     0
    #define NOS_WIN32   0
#elif TARGET_OS_IOS
    #define NOS_ANDROID 0
    #define NOS_IOS     1
    #define NOS_OSX     0
    #define NOS_WIN32   0
#elif TARGET_OS_OSX
    #define NOS_ANDROID 0
    #define NOS_IOS     0
    #define NOS_OSX     1
    #define NOS_WIN32   0
#elif _WIN32
    #define NOS_ANDROID 0
    #define NOS_IOS     0
    #define NOS_OSX     0
    #define NOS_WIN32   1
#else
    #error "unknown os platform."
#endif

//types supported:

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#if !__cplusplus
    //"charxx_t" are not builtin types for c.
    typedef uint16_t char16_t;
    typedef uint32_t char32_t;
#endif

#define nenum(name) typedef int name; enum

nenum(NType) {

    NTYPE_STRUCT = 1, //unknown struct.
    NTYPE_PTR    = 2, //unknown ptr.

    NTYPE_VOID   =  3,
    NTYPE_BOOL   =  4,
    NTYPE_CHAR8  =  5,
    NTYPE_CHAR16 =  6,
    NTYPE_CHAR32 =  7,
    NTYPE_INT8   =  8,
    NTYPE_INT16  =  9,
    NTYPE_INT32  = 10,
    NTYPE_INT64  = 11,
    NTYPE_UINT8  = 12,
    NTYPE_UINT16 = 13,
    NTYPE_UINT32 = 14,
    NTYPE_UINT64 = 15,
    NTYPE_FLOAT  = 16,
    NTYPE_DOUBLE = 17,

    NTYPE_VOID_PTR   = 18,
    NTYPE_BOOL_PTR   = 19,
    NTYPE_CHAR8_PTR  = 20,
    NTYPE_CHAR16_PTR = 21,
    NTYPE_CHAR32_PTR = 22,
    NTYPE_INT8_PTR   = 23,
    NTYPE_INT16_PTR  = 24,
    NTYPE_INT32_PTR  = 25,
    NTYPE_INT64_PTR  = 26,
    NTYPE_UINT8_PTR  = 27,
    NTYPE_UINT16_PTR = 28,
    NTYPE_UINT32_PTR = 29,
    NTYPE_UINT64_PTR = 30,
    NTYPE_FLOAT_PTR  = 31,
    NTYPE_DOUBLE_PTR = 32,

    NTYPE_OBJECT_REF = 33, //NObject ref.
    NTYPE_STRING_REF = 34, //NString ref.

    NTYPE_CUSTOM_REF = 35,

  #if NPTR_64
    NTYPE_INT      = NTYPE_INT64     ,
    NTYPE_UINT     = NTYPE_UINT64    ,
    NTYPE_INT_PTR  = NTYPE_INT64_PTR ,
    NTYPE_UINT_PTR = NTYPE_UINT64_PTR,
  #else
    NTYPE_INT      = NTYPE_INT32     ,
    NTYPE_UINT     = NTYPE_UINT32    ,
    NTYPE_INT_PTR  = NTYPE_INT32_PTR ,
    NTYPE_UINT_PTR = NTYPE_UINT32_PTR,
  #endif
};

#define NTypeIsBlur(t) (t <= NTYPE_PTR )
#define NTypeIsVoid(t) (t == NTYPE_VOID)
#define NTypeIsBool(t) (t == NTYPE_BOOL)

#define NTypeIsNum( t) (NTYPE_CHAR8 <= t && t <= NTYPE_DOUBLE)
#define NTypeIsInt( t) (NTYPE_CHAR8 <= t && t <= NTYPE_UINT64)
#define NTypeIsFlt( t) (NTYPE_FLOAT <= t && t <= NTYPE_DOUBLE)

#define NTypeIsPtr( t) (NTYPE_VOID_PTR   == t && t <= NTYPE_DOUBLE_PTR)
#define NTypeIsRef( t) (NTYPE_OBJECT_REF <= t)

//type modifiers:

#define nsizeof(type) ((int)sizeof(type))

#if NCPL_CL
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

#define _nstruct(name, ...) typedef struct name name; struct name __VA_ARGS__
#define  nstruct(name, ...) _nstruct(name, __VA_ARGS__)

#define _nclass(name, super, ...) typedef struct name name; struct name __VA_ARGS__
#define  nclass(name, super, ...) _nclass(name, super, __VA_ARGS__)

#define _nfunc(ret, name, params) nclink ret  name  params
#define  nfunc(ret, name, params) _nfunc(ret, name, params)
