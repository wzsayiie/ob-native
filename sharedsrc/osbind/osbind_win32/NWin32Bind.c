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

static nthreadlocal int      sCallerArgCount = 0;
static nthreadlocal intptr_t sCallerArgWords[MAX_ARG_NUM] = {0};

void NWINCallerReset(void) {
    sCallerArgCount = 0;
}

static void CallerPush(intptr_t arg) {
    if (sCallerArgCount == MAX_ARG_NUM) {
        _NError("only supports up to %d arguments for win32 api bind", MAX_ARG_NUM);
        return;
    }

    //the argument size of win32 apis will not exceed one cpu word size.
    sCallerArgWords[sCallerArgCount] = arg;
    sCallerArgCount += 1;
}

void NWINCallerPushPtr(const void *arg) {CallerPush((intptr_t)arg);}
void NWINCallerPushInt(int64_t     arg) {CallerPush((intptr_t)arg);}

void NWINCallVoid(NWINApi *api) {
    if (!api) {
        return;
    }

    typedef intptr_t IPTR;
    #define IPTR_0 void
    #define IPTR_1 IPTR
    #define IPTR_2 IPTR,IPTR
    #define IPTR_3 IPTR,IPTR,IPTR
    #define IPTR_4 IPTR,IPTR,IPTR,IPTR
    #define IPTR_5 IPTR,IPTR,IPTR,IPTR,IPTR
    #define IPTR_6 IPTR,IPTR,IPTR,IPTR,IPTR,IPTR
    #define IPTR_7 IPTR,IPTR,IPTR,IPTR,IPTR,IPTR,IPTR
    #define IPTR_8 IPTR,IPTR,IPTR,IPTR,IPTR,IPTR,IPTR,IPTR

    IPTR *word = sCallerArgWords;
    #define word_0
    #define word_1 word[0]
    #define word_2 word[0],word[1]
    #define word_3 word[0],word[1],word[2]
    #define word_4 word[0],word[1],word[2],word[3]
    #define word_5 word[0],word[1],word[2],word[3],word[4]
    #define word_6 word[0],word[1],word[2],word[3],word[4],word[5]
    #define word_7 word[0],word[1],word[2],word[3],word[4],word[5],word[6]
    #define word_8 word[0],word[1],word[2],word[3],word[4],word[5],word[6],word[7]

    switch (sCallerArgCount) {
        case 0 : ((void (*)(IPTR_0))api)(word_0); break;
        case 1 : ((void (*)(IPTR_1))api)(word_1); break;
        case 2 : ((void (*)(IPTR_2))api)(word_2); break;
        case 3 : ((void (*)(IPTR_3))api)(word_3); break;
        case 4 : ((void (*)(IPTR_4))api)(word_4); break;
        case 5 : ((void (*)(IPTR_5))api)(word_5); break;
        case 6 : ((void (*)(IPTR_6))api)(word_6); break;
        case 7 : ((void (*)(IPTR_7))api)(word_7); break;
        case 8 : ((void (*)(IPTR_8))api)(word_8); break;
        default: ;
    }

    //NOTE: clear arguments.
    NWINCallerReset();
}

void *NWINCallPtr(NWINApi *api) {
    if (!api) {
        return NULL;
    }

    typedef intptr_t IPTR;
    #define IPTR_0 void
    #define IPTR_1 IPTR
    #define IPTR_2 IPTR,IPTR
    #define IPTR_3 IPTR,IPTR,IPTR
    #define IPTR_4 IPTR,IPTR,IPTR,IPTR
    #define IPTR_5 IPTR,IPTR,IPTR,IPTR,IPTR
    #define IPTR_6 IPTR,IPTR,IPTR,IPTR,IPTR,IPTR
    #define IPTR_7 IPTR,IPTR,IPTR,IPTR,IPTR,IPTR,IPTR
    #define IPTR_8 IPTR,IPTR,IPTR,IPTR,IPTR,IPTR,IPTR,IPTR

    IPTR *word = sCallerArgWords;
    #define word_0
    #define word_1 word[0]
    #define word_2 word[0],word[1]
    #define word_3 word[0],word[1],word[2]
    #define word_4 word[0],word[1],word[2],word[3]
    #define word_5 word[0],word[1],word[2],word[3],word[4]
    #define word_6 word[0],word[1],word[2],word[3],word[4],word[5]
    #define word_7 word[0],word[1],word[2],word[3],word[4],word[5],word[6]
    #define word_8 word[0],word[1],word[2],word[3],word[4],word[5],word[6],word[7]

    void *ret = NULL;
    switch (sCallerArgCount) {
        case 0 : ret = ((void *(*)(IPTR_0))api)(word_0); break;
        case 1 : ret = ((void *(*)(IPTR_1))api)(word_1); break;
        case 2 : ret = ((void *(*)(IPTR_2))api)(word_2); break;
        case 3 : ret = ((void *(*)(IPTR_3))api)(word_3); break;
        case 4 : ret = ((void *(*)(IPTR_4))api)(word_4); break;
        case 5 : ret = ((void *(*)(IPTR_5))api)(word_5); break;
        case 6 : ret = ((void *(*)(IPTR_6))api)(word_6); break;
        case 7 : ret = ((void *(*)(IPTR_7))api)(word_7); break;
        case 8 : ret = ((void *(*)(IPTR_8))api)(word_8); break;
        default: ;
    }

    //NOTE: clear arguments.
    NWINCallerReset();

    return ret;
}

int64_t NWINCallInt(NWINApi *api) {
    //the return value size of win32 apis will not exceed one cpu word size.
    return (int64_t)NWINCallPtr(api);
}
