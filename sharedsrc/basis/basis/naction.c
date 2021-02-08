#include "naction.h"
#include <stdarg.h>

nclass(NAction, NObject, {
    void    *func;
    NObject *data;
    int      argc;
});

static void ActionClear(NAction *action) {
    NRelease(action->data);
}

static NAction *ActionCreate(void *func, NObject *data, int argc) {
    NAction *action = NCreate(nisizeof(NAction), ActionClear);
    
    action->func = func;
    action->data = NRetain(data);
    action->argc = argc;
    
    return action;
}

NAction *NActionCreateWithFunc3(NActionFunc3 f, NObject *d) {return ActionCreate(f, d, 3);}
NAction *NActionCreateWithFunc2(NActionFunc2 f, NObject *d) {return ActionCreate(f, d, 2);}
NAction *NActionCreateWithFunc1(NActionFunc1 f, NObject *d) {return ActionCreate(f, d, 1);}
NAction *NActionCreateWithFunc (NActionFunc  f, NObject *d) {return ActionCreate(f, d, 0);}
NAction *NActionCreate         ()                           {return ActionCreate(0, 0, 0);}

static void ActionSetFunc(NAction *self, void *func, NObject *data, int argc) {
    if (!self) {
        return;
    }
    
    NRelease(self->data);
    
    self->func = func;
    self->data = NRetain(data);
    self->argc = argc;
}

void NActionSetFunc3(NAction *s, NActionFunc3 f, NObject *d) {ActionSetFunc(s, f, d, 3);}
void NActionSetFunc2(NAction *s, NActionFunc2 f, NObject *d) {ActionSetFunc(s, f, d, 2);}
void NActionSetFunc1(NAction *s, NActionFunc1 f, NObject *d) {ActionSetFunc(s, f, d, 1);}
void NActionSetFunc (NAction *s, NActionFunc  f, NObject *d) {ActionSetFunc(s, f, d, 0);}

void NActionClear(NAction *self) {
    if (self) {
        NRelease(self->data);
        
        self->func = NULL;
        self->data = NULL;
        self->argc = 0;
    }
}

static void ActionRun(NAction *self, int argc, NObject *a, NObject *b, NObject *c) {
    if (!self) {
        return;
    }
    if (!self->func || self->argc != argc) {
        return;
    }
    
    switch (argc) {
        case 3: ((NActionFunc3)self->func)(self->data, a, b, c); break;
        case 2: ((NActionFunc2)self->func)(self->data, a, b); break;
        case 1: ((NActionFunc1)self->func)(self->data, a); break;
        case 0: ((NActionFunc )self->func)(self->data); break;
        default:;
    }
}

#define A3 NObject *a, NObject *b, NObject *c
#define A2 NObject *a, NObject *b
#define A1 NObject *a

void NActionRun3(NAction *s, A3) {ActionRun(s, 3, a, b, c);}
void NActionRun2(NAction *s, A2) {ActionRun(s, 2, a, b, 0);}
void NActionRun1(NAction *s, A1) {ActionRun(s, 1, a, 0, 0);}
void NActionRun (NAction *s    ) {ActionRun(s, 0, 0, 0, 0);}
