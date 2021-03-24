#include "NWin32Bind.h"
#include <Windows.h>

NWINLib *NWINLoadLibrary(NString *dllPath) {
    if (NStringIsEmpty(dllPath)) {
        return NULL;
    }

    const char16_t *path = NStringU16Chars(dllPath);
    HMODULE module = LoadLibraryW(path);
    return (NWINLib *)module;
}

void NWINFreeLibrary(NWINLib *library) {
    if (!library) {
        return;
    }

    HMODULE module = (HMODULE)library;
    FreeLibrary(module);
}

NWINApi *NWINGetProcAddress(NWINLib *library, const char *name) {
    if (!name || *name == '\0') {
        return 0;
    }

    HMODULE module = (HMODULE)library;
    //if no library is specified, search at the current exe file.
    if (!module) {
        module = GetModuleHandleW(NULL);
    }

    FARPROC address = GetProcAddress(module, name);
    return (NWINApi *)address;
}

#define MAX_ARG_NUM 8

static nthreadlocal int      sArgCount = 0;
static nthreadlocal intptr_t sArgWords[MAX_ARG_NUM] = {0};

void NWINApiPrepare(void) {
    sArgCount = 0;
}

static void Push(intptr_t arg) {
    if (sArgCount == MAX_ARG_NUM) {
        _NError("only supports up to %d arguments for win32 api bind", MAX_ARG_NUM);
        return;
    }

    //the argument size of win32 apis will not exceed one cpu word size.
    sArgWords[sArgCount] = arg;
    sArgCount += 1;
}

void NWINApiPushPtr(const void *arg) {Push((intptr_t)arg);}
void NWINApiPushInt(int64_t     arg) {Push((intptr_t)arg);}

#define TYPE_0(T) void
#define TYPE_1(T) T
#define TYPE_2(T) T,T
#define TYPE_3(T) TYPE_2(T),T
#define TYPE_4(T) TYPE_2(T),TYPE_2(T)
#define TYPE_5(T) TYPE_3(T),TYPE_2(T)
#define TYPE_6(T) TYPE_3(T),TYPE_3(T)
#define TYPE_7(T) TYPE_4(T),TYPE_3(T)
#define TYPE_8(T) TYPE_4(T),TYPE_4(T)

#define VAL_0(V)
#define VAL_1(V) V[0]
#define VAL_2(V) V[0],V[1]
#define VAL_3(V) V[0],V[1],V[2]
#define VAL_4(V) V[0],V[1],V[2],V[3]
#define VAL_5(V) V[0],V[1],V[2],V[3],V[4]
#define VAL_6(V) V[0],V[1],V[2],V[3],V[4],V[5]
#define VAL_7(V) V[0],V[1],V[2],V[3],V[4],V[5],V[6]
#define VAL_8(V) V[0],V[1],V[2],V[3],V[4],V[5],V[6],V[7]

void NWINApiCallVoid(NWINApi *api) {
    if (!api) {
        return;
    }

    //NOTE: don't use the cached pass directly, so the lambda can be called recursively.
    int count = sArgCount;
    intptr_t words[MAX_ARG_NUM] = {0};
    NMoveMemory(&words, sArgWords, nsizeof(words));
    NWINApiPrepare();

    switch (count) {
        case 0 : ((void (*)(TYPE_0(intptr_t)))api)(VAL_0(words)); break;
        case 1 : ((void (*)(TYPE_1(intptr_t)))api)(VAL_1(words)); break;
        case 2 : ((void (*)(TYPE_2(intptr_t)))api)(VAL_2(words)); break;
        case 3 : ((void (*)(TYPE_3(intptr_t)))api)(VAL_3(words)); break;
        case 4 : ((void (*)(TYPE_4(intptr_t)))api)(VAL_4(words)); break;
        case 5 : ((void (*)(TYPE_5(intptr_t)))api)(VAL_5(words)); break;
        case 6 : ((void (*)(TYPE_6(intptr_t)))api)(VAL_6(words)); break;
        case 7 : ((void (*)(TYPE_7(intptr_t)))api)(VAL_7(words)); break;
        case 8 : ((void (*)(TYPE_8(intptr_t)))api)(VAL_8(words)); break;
        default: ;
    }
}

void *NWINApiCallPtr(NWINApi *api) {
    if (!api) {
        return NULL;
    }

    //NOTE: don't use the cached pass directly, so the lambda can be called recursively.
    int count = sArgCount;
    intptr_t words[MAX_ARG_NUM] = {0};
    NMoveMemory(&words, sArgWords, nsizeof(words));
    NWINApiPrepare();

    switch (count) {
        case 0 : return ((void *(*)(TYPE_0(intptr_t)))api)(VAL_0(words));
        case 1 : return ((void *(*)(TYPE_1(intptr_t)))api)(VAL_1(words));
        case 2 : return ((void *(*)(TYPE_2(intptr_t)))api)(VAL_2(words));
        case 3 : return ((void *(*)(TYPE_3(intptr_t)))api)(VAL_3(words));
        case 4 : return ((void *(*)(TYPE_4(intptr_t)))api)(VAL_4(words));
        case 5 : return ((void *(*)(TYPE_5(intptr_t)))api)(VAL_5(words));
        case 6 : return ((void *(*)(TYPE_6(intptr_t)))api)(VAL_6(words));
        case 7 : return ((void *(*)(TYPE_7(intptr_t)))api)(VAL_7(words));
        case 8 : return ((void *(*)(TYPE_8(intptr_t)))api)(VAL_8(words));
        default: return NULL;
    }
}

int64_t NWINApiCallInt(NWINApi *api) {
    //the return value size of win32 apis will not exceed one cpu word size.
    return (int64_t)NWINApiCallPtr(api);
}
