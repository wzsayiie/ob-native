#pragma once

//which compiler?
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

//which os?
#ifdef __APPLE__
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

#include <stdbool.h> //bool.
#include <stddef.h>  //NULL.

//NOTE: clang don't provide "uchar.h".
#if (N_COMPILER_CLANG || N_COMPILER_GCC) && !__cplusplus
    typedef unsigned short char16_t;
    typedef unsigned int   char32_t;
#endif

#define nstruct(n) typedef struct n n; struct n
#define nenum(  n) typedef enum   n n; enum   n

#define nisizeof(type) ((int)sizeof(type))

//mutex lock.
void NLock  (int hash);
void NUnlock(int hash);

#define nsynwith(n) for (bool __N = (NLock((int)n), 1); __N; __N = (NUnlock((int)n), 0))
#define nsyn()      nsynwith(__LINE__)
