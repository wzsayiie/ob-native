#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

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

#define defstruct(n) typedef struct n n; struct n
#define defunion( n) typedef union  n n; union  n
#define defenum(  n) typedef enum   n n; enum   n

#define inlinable static inline

# ifdef _MSC_VER
#   define threadlocal __declspec(thread)
# else
#   define threadlocal _Thread_local
# endif

//general delete functin.
//"addr" is address of the structure or pointer.
typedef void (*fdel)(void **addr);
