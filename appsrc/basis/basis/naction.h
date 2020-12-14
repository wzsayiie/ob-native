#pragma once

#include "nmemory.h"

typedef void (*NActionFunc3)(NObject *data, NObject *a, NObject *b, NObject *c);
typedef void (*NActionFunc2)(NObject *data, NObject *a, NObject *b);
typedef void (*NActionFunc1)(NObject *data, NObject *a);
typedef void (*NActionFunc )(NObject *data);

nstruct(NAction);

nfunc(NAction *, NActionCreateWithFunc3, (NActionFunc3 func, NObject *data));
nfunc(NAction *, NActionCreateWithFunc2, (NActionFunc2 func, NObject *data));
nfunc(NAction *, NActionCreateWithFunc1, (NActionFunc1 func, NObject *data));
nfunc(NAction *, NActionCreateWithFunc , (NActionFunc  func, NObject *data));
nfunc(NAction *, NActionCreate         , (void));

nfunc(void, NActionSetFunc3, (NAction *self, NActionFunc3 func, NObject *data));
nfunc(void, NActionSetFunc2, (NAction *self, NActionFunc2 func, NObject *data));
nfunc(void, NActionSetFunc1, (NAction *self, NActionFunc1 func, NObject *data));
nfunc(void, NActionSetFunc , (NAction *self, NActionFunc  func, NObject *data));

nfunc(void, NActionClear, (NAction *self));

nfunc(void, NActionRun3, (NAction *self, NObject *a, NObject *b, NObject *c));
nfunc(void, NActionRun2, (NAction *self, NObject *a, NObject *b));
nfunc(void, NActionRun1, (NAction *self, NObject *a));
nfunc(void, NActionRun , (NAction *self));
