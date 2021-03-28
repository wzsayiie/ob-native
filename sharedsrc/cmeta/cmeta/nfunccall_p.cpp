#include "nfunccall_p.h"
#include "basis.h"
#include "cstr.h"
#include "nfuncmeta_p.h"
#include "ntypemeta_p.h"

template<class Ret>
_Word AsBasic(NType type, _Word word, bool *error) {

    Ret ret[nsizeof(_Word)] = {0};
    switch (type) {
        case NTYPE_BOOL  : *ret = (Ret) *(bool     *)&word; return *(_Word *)ret;
        case NTYPE_CHAR8 : *ret = (Ret) *(char     *)&word; return *(_Word *)ret;
        case NTYPE_CHAR16: *ret = (Ret) *(char16_t *)&word; return *(_Word *)ret;
        case NTYPE_CHAR32: *ret = (Ret) *(char32_t *)&word; return *(_Word *)ret;
        case NTYPE_INT8  : *ret = (Ret) *(int8_t   *)&word; return *(_Word *)ret;
        case NTYPE_INT16 : *ret = (Ret) *(int16_t  *)&word; return *(_Word *)ret;
        case NTYPE_INT32 : *ret = (Ret) *(int32_t  *)&word; return *(_Word *)ret;
        case NTYPE_INT64 : *ret = (Ret) *(int64_t  *)&word; return *(_Word *)ret;
        case NTYPE_UINT8 : *ret = (Ret) *(uint8_t  *)&word; return *(_Word *)ret;
        case NTYPE_UINT16: *ret = (Ret) *(uint16_t *)&word; return *(_Word *)ret;
        case NTYPE_UINT32: *ret = (Ret) *(uint32_t *)&word; return *(_Word *)ret;
        case NTYPE_UINT64: *ret = (Ret) *(uint64_t *)&word; return *(_Word *)ret;
        case NTYPE_FLOAT : *ret = (Ret) *(float    *)&word; return *(_Word *)ret;
        case NTYPE_DOUBLE: *ret = (Ret) *(double   *)&word; return *(_Word *)ret;

        default:;
    }
    *error = true;
    return 0;
}

static _Word AllocChars(NUTFType utf, NType type, _Word word, bool *error) {
    //"FuncPushArg" will not use "NTYPE_CHARX_PTR".
    if (type == NTYPE_STRING) {
        auto string = (NString *)word;
        switch (type) {
            case NUTF8 : return (_Word)csdup  (NStringU8Chars (string));
            case NUTF16: return (_Word)csdup16(NStringU16Chars(string));
            case NUTF32: return (_Word)csdup32(NStringU32Chars(string));
            default    : return 0;
        }
    }
    
    //it's "null".
    if (type == NTYPE_VOID_PTR && word == 0) {
        return 0;
    }
    
    *error = true;
    return 0;
}

static _Word RetainPtr(NType dstType, NType srcType, _Word srcWord, bool *error) {

    //dst and src are both object reference.
    if (dstType >= NTYPE_OBJECT && srcType >= NTYPE_OBJECT) {
        auto object = (NObject *)srcWord;

        for (NType tp = FindType(object->clsName); tp; tp = TypeSuper(tp)) {
            if (tp == dstType) {
                NRetain((NRef) srcWord);
                return srcWord;
            }
        }
    }

    //dst and src are both pointer.
    if (dstType >= NTYPE_VOID_PTR && srcType >= NTYPE_VOID_PTR) {
        //it's "null".
        if (srcType == NTYPE_VOID_PTR && srcWord == 0) {
            return srcWord;
        }
        
        //it's legal that any pointer casted to void pointer.
        if (dstType == NTYPE_VOID_PTR) {
            return srcWord;
        }
        
        if (dstType == srcType) {
            return srcWord;
        }
    }

    *error = true;
    return 0;
}

struct Ticket {
    const char *funcName;
    void       *funcAddress;
    NType       funcRetType;

    int   needArgCount;
    NType needArgTypes[FUNC_MAX_ARG_NUM];

    int   pushArgCount;
    _Word pushArgWords[FUNC_MAX_ARG_NUM];
};

static void SetFuncInfo(Ticket *ticket, int funcIndex) {
    ticket->funcName    = FuncName   (funcIndex);
    ticket->funcAddress = FuncAddress(funcIndex);
    ticket->funcRetType = FuncRetType(funcIndex);

    ticket->needArgCount = FuncArgCount(funcIndex);
    for (int idx = 0; idx < (ticket->needArgCount); ++idx) {
        ticket->needArgTypes[idx] = FuncArgType(funcIndex, idx);
    }
}

static void RetainSuitableArgs(Ticket *ticket, NType *srcTypes, _Word *srcWords) {
    for (int idx = 0; idx < (ticket->needArgCount); ++idx) {

        NType st = srcTypes[idx];
        _Word sw = srcWords[idx];
        NType dt = ticket->needArgTypes[idx];
        _Word dw = 0;

        bool err = false;
        switch (dt) {
            case NTYPE_BLUR_STRUCT: return;
            case NTYPE_BLUR_PTR   : return;
            case NTYPE_VOID       : return;

            case NTYPE_BOOL  : dw = AsBasic<bool    >(st, sw, &err); break;
            case NTYPE_CHAR8 : dw = AsBasic<char    >(st, sw, &err); break;
            case NTYPE_CHAR16: dw = AsBasic<char16_t>(st, sw, &err); break;
            case NTYPE_CHAR32: dw = AsBasic<char32_t>(st, sw, &err); break;
            case NTYPE_INT8  : dw = AsBasic<int8_t  >(st, sw, &err); break;
            case NTYPE_INT16 : dw = AsBasic<int16_t >(st, sw, &err); break;
            case NTYPE_INT32 : dw = AsBasic<int32_t >(st, sw, &err); break;
            case NTYPE_INT64 : dw = AsBasic<int64_t >(st, sw, &err); break;
            case NTYPE_UINT8 : dw = AsBasic<uint8_t >(st, sw, &err); break;
            case NTYPE_UINT16: dw = AsBasic<uint16_t>(st, sw, &err); break;
            case NTYPE_UINT32: dw = AsBasic<uint32_t>(st, sw, &err); break;
            case NTYPE_UINT64: dw = AsBasic<uint64_t>(st, sw, &err); break;
            case NTYPE_FLOAT : dw = AsBasic<float   >(st, sw, &err); break;
            case NTYPE_DOUBLE: dw = AsBasic<double  >(st, sw, &err); break;

            case NTYPE_CHAR8_PTR : dw = AllocChars(NUTF8 , st, sw, &err); break;
            case NTYPE_CHAR16_PTR: dw = AllocChars(NUTF16, st, sw, &err); break;
            case NTYPE_CHAR32_PTR: dw = AllocChars(NUTF32, st, sw, &err); break;

            default: dw = RetainPtr(dt, st, sw, &err);
        }
        if (err) {
            return;
        }

        ticket->pushArgWords[idx] = dw;
        ticket->pushArgCount = idx + 1;
    }
}

static void ReleaseSuitableArgs(Ticket *ticket) {
    for (int idx = 0; idx < (ticket->pushArgCount); ++idx) {
        NType type = ticket->needArgTypes[idx];
        _Word word = ticket->pushArgWords[idx];

        switch (type) {
            case NTYPE_CHAR8_PTR : NFreeMemory((void *)word); break;
            case NTYPE_CHAR16_PTR: NFreeMemory((void *)word); break;
            case NTYPE_CHAR32_PTR: NFreeMemory((void *)word); break;
            case NTYPE_OBJECT    : NRelease   ((NRef  )word); break;
            default:;
        }
    }

    NZeroMemory(ticket->pushArgWords, nsizeof(_Word) * FUNC_MAX_ARG_NUM);
    ticket->pushArgCount = 0;
}

template<class Ret>
struct Executor {

    template<class... Arg>
    static _Word Exec(void *func, Arg... arg) {
        Ret ret[sizeof(_Word)] = {0};
        *ret = ((Ret (*)(Arg...))func)(arg...);
        return *(_Word *)ret;
    }
};

template<>
struct Executor<void> {

    template<class... Arg>
    static _Word Exec(void *func, Arg... arg) {
        ((void (*)(Arg...))func)(arg...);
        return 0;
    }
};

template<class Ret, int N>
struct Caller {

    template<class... Arg>
    static _Word Call(Ticket *ticket, Arg... arg) {

        if (N == ticket->needArgCount) {
            return Executor<Ret>::Exec(ticket->funcAddress, arg...);
        }

        NType type = ticket->needArgTypes[N];
        _Word word = ticket->pushArgWords[N];

        switch (type) {
            case NTYPE_BLUR_STRUCT: return 0;
            case NTYPE_BLUR_PTR   : return 0;

            //only use "intptr_t", "int64_t", "float" and "double" 4 types, to prevent code bloat.
            case NTYPE_INT64 : return Caller<Ret, N + 1>::Call(ticket, arg..., *(int64_t  *)&word);
            case NTYPE_UINT64: return Caller<Ret, N + 1>::Call(ticket, arg..., *(int64_t  *)&word);
            case NTYPE_FLOAT : return Caller<Ret, N + 1>::Call(ticket, arg..., *(float    *)&word);
            case NTYPE_DOUBLE: return Caller<Ret, N + 1>::Call(ticket, arg..., *(double   *)&word);
            default /*short*/: return Caller<Ret, N + 1>::Call(ticket, arg..., *(intptr_t *)&word);
        }
    }
};

template<class Ret>
struct Caller<Ret, FUNC_MAX_ARG_NUM> {

    template<class... Arg>
    static _Word Call(Ticket *ticket, Arg... arg) {
        return Executor<Ret>::Exec(ticket->funcAddress, arg...);
    }
};

static nthreadlocal int   sArgCount = 0;
static nthreadlocal NType sArgTypes[FUNC_MAX_ARG_NUM] = {0};
static nthreadlocal _Word sArgWords[FUNC_MAX_ARG_NUM] = {0};

void FuncPrepare() {
    for (int idx = 0; idx < sArgCount; ++idx) {
        if (sArgTypes[idx] >= NTYPE_OBJECT) {
            NRelease((NRef)sArgWords[idx]);
        }
    }

    NZeroMemory(sArgTypes, nsizeof(NType) * FUNC_MAX_ARG_NUM);
    NZeroMemory(sArgWords, nsizeof(_Word) * FUNC_MAX_ARG_NUM);
    sArgCount = 0;
}

void FuncPushArg(NType type, _Word word) {
    if (sArgCount == FUNC_MAX_ARG_NUM) {
        _NError("dynamic calling only supports up to %d arguments", FUNC_MAX_ARG_NUM);
        return;
    }

    //NOTE: in the case of cross-language calls, it may be difficult for
    //the upper language to hold c objects in the calling lifecycle.
    if (type == NTYPE_CHAR8_PTR) {
        type = NTYPE_STRING;
        if (word) {
            word = (_Word)NStringCreateWithUTFChars(NUTF8 , (void *)word);
        }
    }
    else if (type == NTYPE_CHAR16_PTR) {
        type = NTYPE_STRING;
        if (word) {
            word = (_Word)NStringCreateWithUTFChars(NUTF16, (void *)word);
        }
    }
    else if (type == NTYPE_CHAR32_PTR) {
        type = NTYPE_STRING;
        if (word) {
            word = (_Word)NStringCreateWithUTFChars(NUTF32, (void *)word);
        }
    }
    else if (type >= NTYPE_OBJECT) {
        NRetain((NRef)word);
    }

    sArgTypes[sArgCount] = type;
    sArgWords[sArgCount] = word;
    sArgCount += 1;
}

_Word FuncCall(int funcIndex) {
    Ticket ticket = {0};
    SetFuncInfo(&ticket, funcIndex);
    if (!ticket.funcAddress) {
        _NError("illegal function index %d", funcIndex);
        FuncPrepare();
        return 0;
    }

    if (sArgCount < ticket.needArgCount) {
        const char *name = ticket.funcName;
        const int   need = ticket.needArgCount;
        const int   push = sArgCount;
        _NError("function '%s' needs %d arguments, but only %d passed", name, need, push);
        
        FuncPrepare();
        return 0;
    }

    RetainSuitableArgs(&ticket, sArgTypes, sArgWords);
    if (ticket.pushArgCount < ticket.needArgCount) {
        const char *name = ticket.funcName;
        const int   next = ticket.pushArgCount;
        const char *push = TypeName(sArgTypes[next]);
        const char *need = TypeName(ticket.needArgTypes[next]);
        _NError("'%s' argument %d can't cast '%s' to '%s'", name, next + 1, push, need);
        
        ReleaseSuitableArgs(&ticket);
        FuncPrepare();
        return 0;
    }

    //NOTE: clear cached data, so the function can be called recursively.
    FuncPrepare();

    _Word retWord = 0;
    switch (ticket.funcRetType) {
        case NTYPE_BLUR_STRUCT: break;
        case NTYPE_BLUR_PTR   : break;

        //only use "void", "intptr_t", "int64_t", "float" and "double" 5 types,
        //to prevent code bloat.
        case NTYPE_VOID  : retWord = Caller<void    , 0>::Call(&ticket); break;
        case NTYPE_INT64 : retWord = Caller<int64_t , 0>::Call(&ticket); break;
        case NTYPE_UINT64: retWord = Caller<int64_t , 0>::Call(&ticket); break;
        case NTYPE_FLOAT : retWord = Caller<float   , 0>::Call(&ticket); break;
        case NTYPE_DOUBLE: retWord = Caller<double  , 0>::Call(&ticket); break;
        default /*short*/: retWord = Caller<intptr_t, 0>::Call(&ticket); break;
    }
    ReleaseSuitableArgs(&ticket);
    return retWord;
}
