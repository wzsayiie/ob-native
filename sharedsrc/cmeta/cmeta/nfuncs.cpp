#include "nfuncs.h"
#include "nfunccall_p.h"
#include "nfuncmeta_p.h"

int NFuncsBegin() {
    return FuncsBegin();
}

int NFuncsEnd() {
    return FuncsEnd();
}

int NFindFunc(const char *name) {
    if (name && *name != '\0') {
        return FindFunc(name);
    }
    return 0;
}

const char *NFuncName(int index) {
    return FuncName(index);
}

void *NFuncAddress(int index) {
    return FuncAddress(index);
}

NType NFuncRetType(int index) {
    return FuncRetType(index);
}

bool NFuncRetRetain(int index) {
    return FuncRetRetain(index);
}

int NFuncArgCount(int index) {
    return FuncArgCount(index);
}

NType NFuncArgType(int index, int argIndex) {
    return FuncArgType(index, argIndex);
}

void NFuncPrepare() {
    return FuncPrepare();
}

void NFuncPushArg(NType type, _Word word) {
    return FuncPushArg(type, word);
}

_Word NFuncCall(int index) {
    return FuncCall(index);
}
