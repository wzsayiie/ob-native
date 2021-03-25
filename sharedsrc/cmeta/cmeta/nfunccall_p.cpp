#include "nfunccall_p.h"
#include "basis.h"
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

static _Word AllocU8Chars(NType type, _Word word, bool *error) {
    if (type == NTYPE_CHAR8_PTR ) {return (_Word)NDupU8FromChars(NUTF8 , (void *)word);}
    if (type == NTYPE_CHAR16_PTR) {return (_Word)NDupU8FromChars(NUTF16, (void *)word);}
    if (type == NTYPE_CHAR32_PTR) {return (_Word)NDupU8FromChars(NUTF32, (void *)word);}

    if (type == NTYPE_STRING) {
        const char *chars = NStringU8Chars((NString *)word);
        return (_Word)NDupU8FromChars(NUTF8, chars);
    }

    *error = true;
    return 0;
}

static _Word AllocU16Chars(NType type, _Word word, bool *error) {
    if (type == NTYPE_CHAR8_PTR ) {return (_Word)NDupU16FromChars(NUTF8 , (void *)word);}
    if (type == NTYPE_CHAR16_PTR) {return (_Word)NDupU16FromChars(NUTF16, (void *)word);}
    if (type == NTYPE_CHAR32_PTR) {return (_Word)NDupU16FromChars(NUTF32, (void *)word);}

    if (type == NTYPE_STRING) {
        const char16_t *chars = NStringU16Chars((NString *)word);
        return (_Word)NDupU16FromChars(NUTF16, chars);
    }

    *error = true;
    return 0;
}

static _Word AllocU32Chars(NType type, _Word word, bool *error) {
    if (type == NTYPE_CHAR8_PTR ) {return (_Word)NDupU32FromChars(NUTF8 , (void *)word);}
    if (type == NTYPE_CHAR16_PTR) {return (_Word)NDupU32FromChars(NUTF16, (void *)word);}
    if (type == NTYPE_CHAR32_PTR) {return (_Word)NDupU32FromChars(NUTF32, (void *)word);}

    if (type == NTYPE_STRING) {
        const char32_t *chars = NStringU32Chars((NString *)word);
        return (_Word)NDupU32FromChars(NUTF32, chars);
    }

    *error = true;
    return 0;
}

static _Word RetainString(NType type, _Word word, bool *error) {
    if (type == NTYPE_CHAR8_PTR ) {return (_Word)NStringCreateWithUTFChars(NUTF8 , (void *)word);}
    if (type == NTYPE_CHAR16_PTR) {return (_Word)NStringCreateWithUTFChars(NUTF16, (void *)word);}
    if (type == NTYPE_CHAR32_PTR) {return (_Word)NStringCreateWithUTFChars(NUTF32, (void *)word);}

    if (type == NTYPE_STRING) {
        return (_Word)NRetain((NString *)word);
    }

    *error = true;
    return 0;
}

static _Word AsPtr(NType dstType, NType srcType, _Word word, bool *error) {
    if (srcType >= NTYPE_OBJECT) {
        //is object reference.
        auto object = (NObject *)word;

        for (NType tp = FindType(object->clsName); tp; tp = TypeSuper(tp)) {
            if (tp == dstType) {
                return word;
            }
        }

        *error = true;
        return 0;
    }

    if (srcType >= NTYPE_VOID_PTR) {
        //is pod pointer.
        if (dstType == srcType) {
            return word;
        }

        *error = true;
        return 0;
    }

    *error = true;
    return 0;
}

struct Ticket {
    void *needFuncAddr;
    NType needRetType;
    int   needArgCount;
    NType needArgTypes[FUNC_MAX_ARG_NUM];

    int   pushArgCount;
    _Word pushArgWords[FUNC_MAX_ARG_NUM];
};

static void SetTicketFunc(Ticket *ticket, int funcIndex) {
    ticket->needFuncAddr = FuncAddress (funcIndex);
    ticket->needRetType  = FuncRetType (funcIndex);
    ticket->needArgCount = FuncArgCount(funcIndex);

    for (int idx = 0; idx < (ticket->needArgCount); ++idx) {
        ticket->needArgTypes[idx] = FuncArgType(funcIndex, idx);
    }
}

static void NewTicketArgs(Ticket *ticket, NType *types, _Word *words) {
    for (int idx = 0; idx < (ticket->needArgCount); ++idx) {

        NType srcType = types[idx];
        _Word srcWord = words[idx];
        NType dstType = ticket->needArgTypes[idx];
        _Word dstWord = 0;
        bool  error   = false;

        switch (dstType) {
            case NTYPE_BLUR_STRUCT: return;
            case NTYPE_BLUR_PTR   : return;
            case NTYPE_VOID       : return;

            case NTYPE_BOOL  : dstWord = AsBasic<bool    >(srcType, srcWord, &error); break;
            case NTYPE_CHAR8 : dstWord = AsBasic<char    >(srcType, srcWord, &error); break;
            case NTYPE_CHAR16: dstWord = AsBasic<char16_t>(srcType, srcWord, &error); break;
            case NTYPE_CHAR32: dstWord = AsBasic<char32_t>(srcType, srcWord, &error); break;
            case NTYPE_INT8  : dstWord = AsBasic<int8_t  >(srcType, srcWord, &error); break;
            case NTYPE_INT16 : dstWord = AsBasic<int16_t >(srcType, srcWord, &error); break;
            case NTYPE_INT32 : dstWord = AsBasic<int32_t >(srcType, srcWord, &error); break;
            case NTYPE_INT64 : dstWord = AsBasic<int64_t >(srcType, srcWord, &error); break;
            case NTYPE_UINT8 : dstWord = AsBasic<uint8_t >(srcType, srcWord, &error); break;
            case NTYPE_UINT16: dstWord = AsBasic<uint16_t>(srcType, srcWord, &error); break;
            case NTYPE_UINT32: dstWord = AsBasic<uint32_t>(srcType, srcWord, &error); break;
            case NTYPE_UINT64: dstWord = AsBasic<uint64_t>(srcType, srcWord, &error); break;
            case NTYPE_FLOAT : dstWord = AsBasic<float   >(srcType, srcWord, &error); break;
            case NTYPE_DOUBLE: dstWord = AsBasic<double  >(srcType, srcWord, &error); break;

            case NTYPE_CHAR8_PTR : dstWord = AllocU8Chars (srcType, srcWord, &error); break;
            case NTYPE_CHAR16_PTR: dstWord = AllocU16Chars(srcType, srcWord, &error); break;
            case NTYPE_CHAR32_PTR: dstWord = AllocU32Chars(srcType, srcWord, &error); break;
            case NTYPE_STRING    : dstWord = RetainString (srcType, srcWord, &error); break;

            default: dstWord = AsPtr(dstType, srcType, srcWord, &error);
        }
        if (error) {
            return;
        }

        ticket->pushArgWords[idx] = dstWord;
        ticket->pushArgCount = idx + 1;
    }
}

static void DelTicketArgs(Ticket *ticket) {
    for (int idx = 0; idx < (ticket->pushArgCount); ++idx) {
        NType type = ticket->needArgTypes[idx];
        _Word word = ticket->pushArgWords[idx];

        switch (type) {
            case NTYPE_CHAR8_PTR : NFreeMemory((void *)word); break;
            case NTYPE_CHAR16_PTR: NFreeMemory((void *)word); break;
            case NTYPE_CHAR32_PTR: NFreeMemory((void *)word); break;
            case NTYPE_STRING    : NRelease   ((void *)word); break;
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
            return Executor<Ret>::Exec(ticket->needFuncAddr, arg...);
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
        return Executor<Ret>::Exec(ticket->needFuncAddr, arg...);
    }
};

static nthreadlocal int   sArgCount = 0;
static nthreadlocal NType sArgTypes[FUNC_MAX_ARG_NUM] = {0};
static nthreadlocal _Word sArgWords[FUNC_MAX_ARG_NUM] = {0};

void FuncPrepare() {
    sArgCount = 0;
}

void FuncPushArg(NType type, _Word word) {
    if (sArgCount == FUNC_MAX_ARG_NUM) {
        _NError("only supports up to %d arguments", FUNC_MAX_ARG_NUM);
        return;
    }

    sArgTypes[sArgCount] = type;
    sArgWords[sArgCount] = word;
    sArgCount += 1;
}

_Word FuncCall(int funcIndex) {
    Ticket ticket = {0};
    SetTicketFunc(&ticket, funcIndex);
    if (!ticket.needFuncAddr) {
        _NError("illegal function index %d", funcIndex);
        FuncPrepare();
        return 0;
    }

    if (sArgCount < ticket.needArgCount) {
        _NError("only %d arguments passed, but %d required", sArgCount, ticket.needArgCount);
        FuncPrepare();
        return 0;
    }

    NewTicketArgs(&ticket, sArgTypes, sArgWords);
    if (ticket.pushArgCount < ticket.needArgCount) {
        int   next = ticket.pushArgCount;
        NType push = sArgTypes[next];
        NType need = ticket.needArgTypes[next];

        _NError("argument %d can't cast from type %d to %d", next + 1, push, need);
        
        DelTicketArgs(&ticket);
        FuncPrepare();
        return 0;
    }

    //NOTE: clear cached data, so the function can be called recursively.
    FuncPrepare();

    _Word retWord = 0;
    switch (ticket.needRetType) {
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
    DelTicketArgs(&ticket);
    return retWord;
}
