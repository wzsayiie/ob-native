#pragma once

#include "nmemory.h"
#include "npod.h"
#include "nstring.h"

#define NPASS_MAX_ARG_NUM 4

nstruct(NPass, {
    int    argCount;
    NValue argValues[NPASS_MAX_ARG_NUM];
    NValue retValue;
});

nfunc(int, NPassArgCount, (NPass *pass));

nfunc(bool            , NPassArgBool    , (NPass *pass, int index));
nfunc(int             , NPassArgInt     , (NPass *pass, int index));
nfunc(int64_t         , NPassArgInt64   , (NPass *pass, int index));
nfunc(unsigned        , NPassArgUInt    , (NPass *pass, int index));
nfunc(uint64_t        , NPassArgUInt64  , (NPass *pass, int index));
nfunc(float           , NPassArgFloat   , (NPass *pass, int index));
nfunc(double          , NPassArgDouble  , (NPass *pass, int index));
nfunc(void           *, NPassArgPtr     , (NPass *pass, int index));
nfunc(const char     *, NPassArgU8Chars , (NPass *pass, int index));
nfunc(const char16_t *, NPassArgU16Chars, (NPass *pass, int index));
nfunc(const char32_t *, NPassArgU32Chars, (NPass *pass, int index));
nfunc(NRef            , NPassArgObject  , (NPass *pass, int index));
nfunc(NString        *, NPassArgString  , (NPass *pass, int index));
nfunc(NLambda        *, NPassArgLambda  , (NPass *pass, int index));

nfunc(void, NPassReturnBool    , (NPass *pass, bool            ret));
nfunc(void, NPassReturnInt     , (NPass *pass, int             ret));
nfunc(void, NPassReturnInt64   , (NPass *pass, int64_t         ret));
nfunc(void, NPassReturnUInt    , (NPass *pass, unsigned        ret));
nfunc(void, NPassReturnUInt64  , (NPass *pass, uint64_t        ret));
nfunc(void, NPassReturnFloat   , (NPass *pass, float           ret));
nfunc(void, NPassReturnDouble  , (NPass *pass, double          ret));
nfunc(void, NPassReturnPtr     , (NPass *pass, void           *ret));
nfunc(void, NPassReturnU8Chars , (NPass *pass, const char     *ret));
nfunc(void, NPassReturnU16Chars, (NPass *pass, const char16_t *ret));
nfunc(void, NPassReturnU32Chars, (NPass *pass, const char32_t *ret));
nfunc(void, NPassReturnObject  , (NPass *pass, NRef            ret));
nfunc(void, NPassReturnString  , (NPass *pass, NString        *ret));
nfunc(void, NPassReturnLambda  , (NPass *pass, NLambda        *ret));

typedef void (*NLambdaFunc)(NLambda *lambda, NPass *pass);

nclass(NLambda, NObject, {
    union {
        NObject Object;
        NObject Super ;
    };

    NRef  customObject;
    void *customData;

    NLambdaFunc func;
    NPass pass;
});

void _NLambdaInitWithObject(NLambda *lambda, NRef  object, NLambdaFunc func);
void _NLambdaInitWithData  (NLambda *lambda, void *data  , NLambdaFunc func);
void _NLambdaInit          (NLambda *lambda);
void _NLambdaDeinit        (NLambda *lambda);

nfunc(NLambda *, NLambdaCreateWithObject, (NRef  object, NLambdaFunc func));
nfunc(NLambda *, NLambdaCreateWithData  , (void *data  , NLambdaFunc func));
nfunc(NLambda *, NLambdaCreate          , (void));

nfunc(void, NLambdaSetFunc  , (NLambda *lambda, NLambdaFunc func));
nfunc(void, NLambdaSetObject, (NLambda *lambda, NRef object));
nfunc(void, NLambdaSetData  , (NLambda *lambda, void *data));

nfunc(void, NLambdaPrepareCall, (NLambda *lambda));

nfunc(void, NLambdaPushBool    , (NLambda *lambda, bool            arg));
nfunc(void, NLambdaPushInt     , (NLambda *lambda, int             arg));
nfunc(void, NLambdaPushInt64   , (NLambda *lambda, int64_t         arg));
nfunc(void, NLambdaPushUInt    , (NLambda *lambda, unsigned        arg));
nfunc(void, NLambdaPushUInt64  , (NLambda *lambda, uint64_t        arg));
nfunc(void, NLambdaPushFloat   , (NLambda *lambda, float           arg));
nfunc(void, NLambdaPushDouble  , (NLambda *lambda, double          arg));
nfunc(void, NLambdaPushPtr     , (NLambda *lambda, void           *arg));
nfunc(void, NLambdaPushU8Chars , (NLambda *lambda, const char     *arg));
nfunc(void, NLambdaPushU16Chars, (NLambda *lambda, const char16_t *arg));
nfunc(void, NLambdaPushU32Chars, (NLambda *lambda, const char32_t *arg));
nfunc(void, NLambdaPushObject  , (NLambda *lambda, NRef           *arg));
nfunc(void, NLambdaPushString  , (NLambda *lambda, NString        *arg));
nfunc(void, NLambdaPushLambda  , (NLambda *lambda, NLambda        *arg));

nfunc(void            , NLambdaCallVoid    , (NLambda *lambda));
nfunc(bool            , NLambdaCallBool    , (NLambda *lambda));
nfunc(int             , NLambdaCallInt     , (NLambda *lambda));
nfunc(int64_t         , NLambdaCallInt64   , (NLambda *lambda));
nfunc(unsigned        , NLambdaCallUInt    , (NLambda *lambda));
nfunc(uint64_t        , NLambdaCallUInt64  , (NLambda *lambda));
nfunc(float           , NLambdaCallFloat   , (NLambda *lambda));
nfunc(double          , NLambdaCallDouble  , (NLambda *lambda));
nfunc(void           *, NLambdaCallPtr     , (NLambda *lambda));
nfunc(const char     *, NLambdaCallU8Chars , (NLambda *lambda));
nfunc(const char16_t *, NLambdaCallU16Chars, (NLambda *lambda));
nfunc(const char32_t *, NLambdaCallU32Chars, (NLambda *lambda));
nfunc(NRef            , NLambdaCallObject  , (NLambda *lambda));
nfunc(NString        *, NLambdaCallString  , (NLambda *lambda));
nfunc(NLambda        *, NLambdaCallLambda  , (NLambda *lambda));
