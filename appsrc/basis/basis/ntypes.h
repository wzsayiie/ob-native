#pragma once

#include "nenviron.h"
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

#if N_COMPILER_CL
    #define nthreadlocal __declspec(thread)
#else
    #define nthreadlocal _Thread_local
#endif

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

nstruct(NValue) {
    union {
        int64_t intValue;
        double  fltValue;
        void   *ptrValue;
    };
};

#if __cplusplus
    #define nclink extern "C"
    #define __nfunc(ret, name, params) nclink ret name params
#else
    #define nclink
    #define __nfunc(ret, name, params) ret name params
#endif

//the flag for generating function meta data.
#define nfunc(ret, name, params) __nfunc(ret, name, params)
