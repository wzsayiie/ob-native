#pragma once

#include "nmemory.h"

typedef void (*NActionFunc3)(NRef data, NRef a, NRef b, NRef c);
typedef void (*NActionFunc2)(NRef data, NRef a, NRef b);
typedef void (*NActionFunc1)(NRef data, NRef a);
typedef void (*NActionFunc )(NRef data);

nclass(NAction, NObject, {
    union {
        NObject Object;
        NObject Super ;
    };

    void *func;
    NRef  data;
    int   argc;
});

void _NActionInitWithFunc3(NAction *action, NActionFunc3 func, NRef data);
void _NActionInitWithFunc2(NAction *action, NActionFunc2 func, NRef data);
void _NActionInitWithFunc1(NAction *action, NActionFunc1 func, NRef data);
void _NActionInitWithFunc (NAction *action, NActionFunc  func, NRef data);
void _NActionInit         (NAction *action);
void _NActionDeinit       (NAction *action);

nfunc(NAction *, NActionCreateWithFunc3, (NActionFunc3 func, NRef data));
nfunc(NAction *, NActionCreateWithFunc2, (NActionFunc2 func, NRef data));
nfunc(NAction *, NActionCreateWithFunc1, (NActionFunc1 func, NRef data));
nfunc(NAction *, NActionCreateWithFunc , (NActionFunc  func, NRef data));
nfunc(NAction *, NActionCreate         , (void));

nfunc(void, NActionSetFunc3, (NAction *action, NActionFunc3 func, NRef data));
nfunc(void, NActionSetFunc2, (NAction *action, NActionFunc2 func, NRef data));
nfunc(void, NActionSetFunc1, (NAction *action, NActionFunc1 func, NRef data));
nfunc(void, NActionSetFunc , (NAction *action, NActionFunc  func, NRef data));

nfunc(void, NActionClear, (NAction *action));

nfunc(void, NActionRun3, (NAction *action, NRef a, NRef b, NRef c));
nfunc(void, NActionRun2, (NAction *action, NRef a, NRef b));
nfunc(void, NActionRun1, (NAction *action, NRef a));
nfunc(void, NActionRun , (NAction *action));
