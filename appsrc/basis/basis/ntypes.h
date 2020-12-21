#pragma once

#include "nenviron.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#if N_PTR_64
    typedef int64_t  npbool;
    typedef int64_t  npint ;
    typedef uint64_t npuint;
    typedef double   npflt ;
#else
    typedef int32_t  npbool;
    typedef int32_t  npint ;
    typedef uint32_t npuint;
    typedef float    npflt ;
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

  #if N_PTR_64
    NTypeInt  = NTypeInt64 ,
    NTypeUInt = NTypeUInt64,
  #else
    NTypeInt  = NTypeInt32 ,
    NTypeUInt = NTypeUInt32,
  #endif
};

nstruct(NWord) {
    union {
        bool      asBool  ;
        int8_t    asInt8  ;
        int16_t   asInt16 ;
        int32_t   asInt32 ;
        int64_t   asInt64 ;
        int       asInt   ;
        uint8_t   asUInt8 ;
        uint16_t  asUInt16;
        uint32_t  asUInt32;
        uint64_t  asUInt64;
        unsigned  asUInt  ;
        float     asFloat ;
        double    asDouble;
        void     *asPtr   ;
    };
};

nstruct(NValue) {
    union {
        bool     boolValue;
        int64_t  intValue ;
        uint64_t uintValue;
        double   dblValue ;
        void    *ptrValue ;
    };
    NType type;
};

#if __cplusplus
    #define nclink extern "C"
#else
    #define nclink
#endif

nclink NValue NMakeBoolValue(bool     value);
nclink NValue NMakeIntValue (int64_t  value);
nclink NValue NMakeUIntValue(uint64_t value);
nclink NValue NMakeDblValue (double   value);
nclink NValue NMakePtrValue (void    *value);

nclink bool     NBoolValue(NValue value);
nclink int64_t  NIntValue (NValue value);
nclink uint64_t NUIntValue(NValue value);
nclink double   NDblValue (NValue value);
nclink void    *NPtrValue (NValue value);

//the flag for generating function meta data.
#define __nfunc(ret, name, params) nclink  ret  name  params
#define   nfunc(ret, name, params) __nfunc(ret, name, params)
