#include "naction.h"
#include <stdarg.h>

#define GEN_INIT(A, F, D, C) _NObjectInit(nsuperof(A)); A->func = F; A->data = D; A->argc = C;

void _NActionInitWithFunc3(NAction *act, NActionFunc3 func, NRef data) {GEN_INIT(act, func, data, 3)}
void _NActionInitWithFunc2(NAction *act, NActionFunc2 func, NRef data) {GEN_INIT(act, func, data, 2)}
void _NActionInitWithFunc1(NAction *act, NActionFunc1 func, NRef data) {GEN_INIT(act, func, data, 1)}
void _NActionInitWithFunc (NAction *act, NActionFunc  func, NRef data) {GEN_INIT(act, func, data, 0)}
void _NActionInit         (NAction *act                              ) {GEN_INIT(act, 0   , 0   , 0)}

void _NActionDeinit(NAction *action) {
    NRelease(action->data);
    _NObjectDeinit(nsuperof(action));
}

#define GEN_CREATE(I, ...) NAction *a = NAlloc(NAction, _NActionDeinit); I(a, ##__VA_ARGS__); return a;

NAction *NActionCreateWithFunc3(NActionFunc3 func, NRef data) {GEN_CREATE(_NActionInitWithFunc3, func, data)}
NAction *NActionCreateWithFunc2(NActionFunc2 func, NRef data) {GEN_CREATE(_NActionInitWithFunc2, func, data)}
NAction *NActionCreateWithFunc1(NActionFunc1 func, NRef data) {GEN_CREATE(_NActionInitWithFunc1, func, data)}
NAction *NActionCreateWithFunc (NActionFunc  func, NRef data) {GEN_CREATE(_NActionInitWithFunc , func, data)}
NAction *NActionCreate         ()                             {GEN_CREATE(_NActionInit                     )}

static void ActionSetFunc(NAction *action, void *func, NRef data, int argc) {
    if (!action) {
        return;
    }
    
    NRelease(action->data);
    
    action->func = func;
    action->data = NRetain(data);
    action->argc = argc;
}

void NActionSetFunc3(NAction *action, NActionFunc3 func, NRef data) {ActionSetFunc(action, func, data, 3);}
void NActionSetFunc2(NAction *action, NActionFunc2 func, NRef data) {ActionSetFunc(action, func, data, 2);}
void NActionSetFunc1(NAction *action, NActionFunc1 func, NRef data) {ActionSetFunc(action, func, data, 1);}
void NActionSetFunc (NAction *action, NActionFunc  func, NRef data) {ActionSetFunc(action, func, data, 0);}

void NActionClear(NAction *action) {
    if (!action) {
        return;
    }
    
    NRelease(action->data);
    
    action->func = NULL;
    action->data = NULL;
    action->argc = 0;
}

static void ActionRun(NAction *action, int argc, NRef a, NRef b, NRef c) {
    if (!action) {
        return;
    }
    if (!action->func || argc < action->argc) {
        return;
    }
    
    switch (action->argc) {
        case 3: ((NActionFunc3)action->func)(action->data, a, b, c); break;
        case 2: ((NActionFunc2)action->func)(action->data, a, b); break;
        case 1: ((NActionFunc1)action->func)(action->data, a); break;
        case 0: ((NActionFunc )action->func)(action->data); break;
        default:;
    }
}

void NActionRun3(NAction *action, NRef a, NRef b, NRef c) {ActionRun(action, 3, a, b, c);}
void NActionRun2(NAction *action, NRef a, NRef b)         {ActionRun(action, 2, a, b, 0);}
void NActionRun1(NAction *action, NRef a)                 {ActionRun(action, 1, a, 0, 0);}
void NActionRun (NAction *action)                         {ActionRun(action, 0, 0, 0, 0);}
