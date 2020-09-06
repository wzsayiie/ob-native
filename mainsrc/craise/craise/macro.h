#pragma once

#include <stddef.h>

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

#define _struct(n) typedef struct n n; struct n
#define _union( n) typedef union  n n; union  n
#define _enum(  n) typedef enum   n n; enum   n

#define _inline static inline

# ifdef _MSC_VER
#   define _threadlocal __declspec(thread)
# else
#   define _threadlocal _Thread_local
# endif

//range-based traversal.
//the last parameter "step" is optional, it's default value is 1.
#define _forrange(var, begin, end, ...) \
/**/    for (                           \
/**/        int var = begin;            \
/**/        (1, ##__VA_ARGS__) > 0 && var < end || (1, ##__VA_ARGS__) < 0 && var > end; \
/**/        var += (1, ##__VA_ARGS__)   \
/**/    )

//general delete functin.
//"addr" is address of the structure or pointer.
typedef void (*fdel)(void **addr);
