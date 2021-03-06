#include "naction.h"
#include <stdarg.h>

void _NActionInitWithObject(NAction *action, NActionFunc func, NRef object) {
    _NObjectInit(nsuperof(action));
    action->func = func;
    action->object = NRetain(object);
}

void _NActionInit(NAction *action) {
    _NObjectInit(nsuperof(action));
}

void _NActionDeinit(NAction *action) {
    NRelease(action->object);
    _NObjectDeinit(nsuperof(action));
}

NAction *NActionCreateWithObject(NActionFunc func, NRef object) {
    NAction *action = NAlloc(NAction, _NActionDeinit);
    _NActionInitWithObject(action, func, object);
    return action;
}

NAction *NActionCreate(void) {
    NAction *action = NAlloc(NAction, _NActionDeinit);
    _NActionInit(action);
    return action;
}

void NActionResetObject(NAction *action, NActionFunc func, NRef object) {
    if (!action) {
        return;
    }
    
    NRelease(action->object);

    action->func = func;
    action->object = NRetain(object);
}

void NActionRun(NAction *action, void *arg) {
    if (action && action->func) {
        action->func(action->object, arg);
    }
}
