#pragma once

#include "nmemory.h"

typedef void (*NActionFunc)(NRef object, void *arg);

nclass(NAction, NObject, {
    union {
        NObject Object;
        NObject Super ;
    };

    NActionFunc func;
    NRef *object;
});

void _NActionInitWithObject(NAction *action, NActionFunc func, NRef object);
void _NActionInit          (NAction *action);
void _NActionDeinit        (NAction *action);

nfunc(NAction *, NActionCreateWithObject, (NActionFunc func, NRef object));
nfunc(NAction *, NActionCreate          , (void));

nfunc(void, NActionResetObject, (NAction *action, NActionFunc func, NRef object));

nfunc(void, NActionRun, (NAction *action, void *arg));
