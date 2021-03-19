#pragma once

#if !__cplusplus
    #error "c++ needed"
#endif

#include "ntrait_p.h"

static const int FUNC_MAX_ARG_NUM = 4;

struct FuncEntry {
    const char *funcName;
    void       *funcAddr;
    const char *retName;
    NType       retType;
    bool        retRetain;
    int         argCount;
    const char *argNames[FUNC_MAX_ARG_NUM];
    NType       argTypes[FUNC_MAX_ARG_NUM];
};

struct FuncAdder {

    template<class Ret, class... Arg>
    FuncAdder(const char *name, Ret (*func)(Arg...)) {
        bool retOkay = CheckRet<Ret>();
        if (!retOkay) {
            return;
        }

        int argCount = 0;
        bool argOkay = CheckArg(func, &argCount);
        if (!argOkay) {
            return;
        }
        if (argCount > FUNC_MAX_ARG_NUM) {
            return;
        }
        
        FuncEntry entry = {0};

        entry.funcName  = name;
        entry.funcAddr  = (void *)func;
        entry.retName   = TraitName<Ret>::NAME;
        entry.retType   = TraitType<Ret>::TYPE;
        entry.retRetain = RetRetain(entry.retType, name);
        
        entry.argCount = 0;
        AssignArg(&entry, func);
        
        AddEntry(&entry);
    }

    template<class Ret>
    bool CheckRet() {
        //the return type can't be a struct or unknown ptr.
        if (TraitType<Ret>::TYPE == NTYPE_STRUCT) {return false;}
        if (TraitType<Ret>::TYPE == NTYPE_PTR   ) {return false;}

        return true;
    }

    template<class Ret, class First, class... Follow>
    bool CheckArg(Ret (*)(First, Follow...), int *count) {
        if (TraitType<First>::TYPE == NTYPE_STRUCT) {return false;}
        if (TraitType<First>::TYPE == NTYPE_PTR   ) {return false;}
        if (TraitType<First>::TYPE == NTYPE_VOID  ) {return false;}

        *count += 1;
        return CheckArg((Ret (*)(Follow...))NULL, count);
    }

    template<class Ret>
    bool CheckArg(Ret (*)(), int *) {
        return true;
    }

    template<class Ret, class First, class... Follow>
    void AssignArg(FuncEntry *entry, Ret (*)(First, Follow...)) {
        int index = (entry->argCount)++;

        entry->argNames[index] = TraitName<First>::NAME;
        entry->argTypes[index] = TraitType<First>::TYPE;

        AssignArg(entry, (void (*)(Follow...))NULL);
    }

    template<class Ret>
    void AssignArg(FuncEntry *, Ret (*)()) {
    }

    static bool RetRetain(NType retType, const char *funcName);
    static void AddEntry(FuncEntry *entry);
};

#define AddFunc(name) static FuncAdder _func_##name(#name, name)

int FuncsBegin();
int FuncsEnd  ();
int FindFunc  (const char *name);

const char *FuncName(int index);

void *FuncAddress  (int index);
NType FuncRetType  (int index);
bool  FuncRetRetain(int index);
int   FuncArgCount (int index);
NType FuncArgType  (int index, int argIndex);
