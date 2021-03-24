#include "nlambda.h"

int NPassArgCount(NPass *pass) {
    if (pass) {
        return pass->argCount;
    }
    return 0;
}

static NValue Arg(NPass *pass, int index) {
    if (pass) {
        if (0 <= index && index < pass->argCount) {
            return pass->unretainArgs[index];
        }
    }
    return NMakeVoidValue();
}

bool            NPassArgBool    (NPass *pas, int idx) {return NBoolValue    (Arg(pas, idx));}
int             NPassArgInt     (NPass *pas, int idx) {return NIntValue     (Arg(pas, idx));}
int64_t         NPassArgInt64   (NPass *pas, int idx) {return NInt64Value   (Arg(pas, idx));}
unsigned        NPassArgUInt    (NPass *pas, int idx) {return NUIntValue    (Arg(pas, idx));}
uint64_t        NPassArgUInt64  (NPass *pas, int idx) {return NUInt64Value  (Arg(pas, idx));}
float           NPassArgFloat   (NPass *pas, int idx) {return NFloatValue   (Arg(pas, idx));}
double          NPassArgDouble  (NPass *pas, int idx) {return NDoubleValue  (Arg(pas, idx));}
void           *NPassArgPtr     (NPass *pas, int idx) {return NPtrValue     (Arg(pas, idx));}
const char     *NPassArgU8Chars (NPass *pas, int idx) {return NU8CharsValue (Arg(pas, idx));}
const char16_t *NPassArgU16Chars(NPass *pas, int idx) {return NU16CharsValue(Arg(pas, idx));}
const char32_t *NPassArgU32Chars(NPass *pas, int idx) {return NU32CharsValue(Arg(pas, idx));}
NRef            NPassArgObject  (NPass *pas, int idx) {return NObjectValue  (Arg(pas, idx));}
NString        *NPassArgString  (NPass *pas, int idx) {return NStringValue  (Arg(pas, idx));}
NLambda        *NPassArgLambda  (NPass *pas, int idx) {return NLambdaValue  (Arg(pas, idx));}

static void SetRet(NPass *pass, NValue ret) {
    NValue *value = &pass->retainedRet;

    if (NTypeIsRef(value->type)) {
        NRelease(value->asObject);
    }

    if (NTypeIsRef(ret.type)) {
        NRetain(ret.asObject);
    }
    *value = ret;
}

void NPassReturnBool  (NPass *pas, bool     ret) {if (pas) SetRet(pas, NMakeBoolValue  (ret));}
void NPassReturnInt   (NPass *pas, int      ret) {if (pas) SetRet(pas, NMakeIntValue   (ret));}
void NPassReturnInt64 (NPass *pas, int64_t  ret) {if (pas) SetRet(pas, NMakeInt64Value (ret));}
void NPassReturnUInt  (NPass *pas, unsigned ret) {if (pas) SetRet(pas, NMakeUIntValue  (ret));}
void NPassReturnUInt64(NPass *pas, uint64_t ret) {if (pas) SetRet(pas, NMakeUInt64Value(ret));}
void NPassReturnFloat (NPass *pas, float    ret) {if (pas) SetRet(pas, NMakeFloatValue (ret));}
void NPassReturnDouble(NPass *pas, double   ret) {if (pas) SetRet(pas, NMakeDoubleValue(ret));}
void NPassReturnPtr   (NPass *pas, void    *ret) {if (pas) SetRet(pas, NMakePtrValue   (ret));}

static void SetStrRet(NPass *pass, NUTFType type, const void *chars) {
    NString *string = NULL;
    if (chars) {
        string = NStringCreateWithUTFChars(type, chars);
    }

    SetRet(pass, NMakeStringValue(string));
}

void NPassReturnU8Chars (NPass *pas, const char     *ret) {if (pas) SetStrRet(pas, NUTF8 , ret);}
void NPassReturnU16Chars(NPass *pas, const char16_t *ret) {if (pas) SetStrRet(pas, NUTF16, ret);}
void NPassReturnU32Chars(NPass *pas, const char32_t *ret) {if (pas) SetStrRet(pas, NUTF32, ret);}

void NPassReturnObject(NPass *pas, NRef     ret) {if (pas) SetRet(pas, NMakeObjectValue(ret));}
void NPassReturnString(NPass *pas, NString *ret) {if (pas) SetRet(pas, NMakeStringValue(ret));}
void NPassReturnLambda(NPass *pas, NLambda *ret) {if (pas) SetRet(pas, NMakeLambdaValue(ret));}

void _NLambdaInitWithObject(NLambda *lambda, NRef object, NLambdaFunc func) {
    _NObjectInit(nsuperof(lambda));

    lambda->customObject = NRetain(object);
    lambda->func = func;
}

void _NLambdaInitWithData(NLambda *lambda, void *data, NLambdaFunc func) {
    _NObjectInit(nsuperof(lambda));

    lambda->customData = data;
    lambda->func = func;
}

void _NLambdaInit(NLambda *lambda) {
    _NObjectInit(nsuperof(lambda));
}

static void ReleaseRet(NLambda *lambda) {
    NValue *value = &lambda->pass.retainedRet;

    if (NTypeIsRef(value->type)) {
        NRelease(value->asObject);
        *value = NMakeVoidValue();
    }
}

void _NLambdaDeinit(NLambda *lambda) {
    NRelease(lambda->customObject);
    ReleaseRet(lambda);

    _NObjectDeinit(nsuperof(lambda));
}

NLambda *NLambdaCreateWithObject(NRef object, NLambdaFunc func) {
    NLambda *lambda = NAlloc(NLambda, _NLambdaDeinit);
    _NLambdaInitWithObject(lambda, object, func);
    return lambda;
}

NLambda *NLambdaCreateWithData(void *data, NLambdaFunc func) {
    NLambda *lambda = NAlloc(NLambda, _NLambdaDeinit);
    _NLambdaInitWithData(lambda, data, func);
    return lambda;
}

NLambda *NLambdaCreate(void) {
    NLambda *lambda = NAlloc(NLambda, _NLambdaDeinit);
    _NLambdaInit(lambda);
    return lambda;
}

void NLambdaSetFunc(NLambda *lambda, NLambdaFunc func) {
    if (lambda) {
        lambda->func = func;
    }
}

void NLambdaSetObject(NLambda *lambda, NRef object) {
    if (lambda) {
        NRelease(lambda->customObject);
        lambda->customObject = NRetain(object);
    }
}

void NLambdaSetData(NLambda *lambda, void *data) {
    if (lambda) {
        lambda->customData = data;
    }
}

void NLambdaPrepareCall(NLambda *lambda) {
    if (lambda) {
        ReleaseRet(lambda);
        NZeroMemory(&lambda->pass, nsizeof(NPass));
    }
}

static void PushArg(NLambda *lambda, NValue arg) {
    if (lambda->pass.argCount == NPASS_MAX_ARG_NUM) {
        return;
    }

    lambda->pass.unretainArgs[lambda->pass.argCount] = arg;
    lambda->pass.argCount += 1;
}

void NLambdaPushBool    (NLambda *lam, bool            arg) {if (lam) PushArg(lam, NMakeBoolValue    (arg));}
void NLambdaPushInt     (NLambda *lam, int             arg) {if (lam) PushArg(lam, NMakeIntValue     (arg));}
void NLambdaPushInt64   (NLambda *lam, int64_t         arg) {if (lam) PushArg(lam, NMakeInt64Value   (arg));}
void NLambdaPushUInt    (NLambda *lam, unsigned        arg) {if (lam) PushArg(lam, NMakeUIntValue    (arg));}
void NLambdaPushUInt64  (NLambda *lam, uint64_t        arg) {if (lam) PushArg(lam, NMakeUInt64Value  (arg));}
void NLambdaPushFloat   (NLambda *lam, float           arg) {if (lam) PushArg(lam, NMakeFloatValue   (arg));}
void NLambdaPushDouble  (NLambda *lam, double          arg) {if (lam) PushArg(lam, NMakeDoubleValue  (arg));}
void NLambdaPushPtr     (NLambda *lam, void           *arg) {if (lam) PushArg(lam, NMakePtrValue     (arg));}
void NLambdaPushU8Chars (NLambda *lam, const char     *arg) {if (lam) PushArg(lam, NMakeU8CharsValue (arg));}
void NLambdaPushU16Chars(NLambda *lam, const char16_t *arg) {if (lam) PushArg(lam, NMakeU16CharsValue(arg));}
void NLambdaPushU32Chars(NLambda *lam, const char32_t *arg) {if (lam) PushArg(lam, NMakeU32CharsValue(arg));}
void NLambdaPushObject  (NLambda *lam, NRef           *arg) {if (lam) PushArg(lam, NMakeObjectValue  (arg));}
void NLambdaPushString  (NLambda *lam, NString        *arg) {if (lam) PushArg(lam, NMakeStringValue  (arg));}
void NLambdaPushLambda  (NLambda *lam, NLambda        *arg) {if (lam) PushArg(lam, NMakeLambdaValue  (arg));}

static NValue Call(NLambda *lambda) {
    if (!lambda || !lambda->func) {
        return NMakeVoidValue();
    }

    ReleaseRet(lambda);

    //NOTE: don't use the cached pass directly, so the lambda can be called recursively.
    NPass pass = lambda->pass;
    NZeroMemory(&lambda->pass, nsizeof(NPass));

    lambda->func(lambda, &pass);
    lambda->pass = pass;
    return lambda->pass.retainedRet;
}

void     NLambdaCallVoid  (NLambda *lam) {/*.... NVoidValue*/(Call(lam));}
bool     NLambdaCallBool  (NLambda *lam) {return NBoolValue  (Call(lam));}
int      NLambdaCallInt   (NLambda *lam) {return NIntValue   (Call(lam));}
int64_t  NLambdaCallInt64 (NLambda *lam) {return NInt64Value (Call(lam));}
unsigned NLambdaCallUInt  (NLambda *lam) {return NUIntValue  (Call(lam));}
uint64_t NLambdaCallUInt64(NLambda *lam) {return NUInt64Value(Call(lam));}
float    NLambdaCallFloat (NLambda *lam) {return NFloatValue (Call(lam));}
double   NLambdaCallDouble(NLambda *lam) {return NDoubleValue(Call(lam));}
void    *NLambdaCallPtr   (NLambda *lam) {return NPtrValue   (Call(lam));}
NRef     NLambdaCallObject(NLambda *lam) {return NObjectValue(Call(lam));}
NString *NLambdaCallString(NLambda *lam) {return NStringValue(Call(lam));}
NLambda *NLambdaCallLambda(NLambda *lam) {return NLambdaValue(Call(lam));}
