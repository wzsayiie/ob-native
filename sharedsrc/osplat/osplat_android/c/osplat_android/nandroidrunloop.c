#include "nrunloop.h"
#include "cmeta.h"
#include "osbind.h"

static NJNIClass *RunLoopClass(void) {return NJNIImportClass("src/shared/osplat/NRunLoop");}
static NJNIClass *ValueClass  (void) {return NJNIImportClass("src/shared/osbind/NValue"  );}
static NJNIClass *IntClass    (void) {return NJNIImportClass("int"    );}
static NJNIClass *LongClass   (void) {return NJNIImportClass("long"   );}
static NJNIClass *BooleanClass(void) {return NJNIImportClass("boolean");}

//mutex is not necessary here,
//cause the value obtained by each thread is the same.

static NJNIObject *CreateJNIAction(NAction *action) {
    static NJNIMethod *holdMethod = NULL;
    static NType actionType = 0;
    if (!holdMethod) {
        NJNIFinderReset();
        NJNIFinderSetRet(ValueClass  ());
        NJNIFinderAddArg(IntClass    ());
        NJNIFinderAddArg(LongClass   ());
        NJNIFinderAddArg(BooleanClass());
        holdMethod = NJNIFindStaticMethod(ValueClass(), "hold");

        actionType = NFindStruct("NAction");
    }

    NRetain(action);
    bool needRelease = true;

    NJNICallerReset();
    NJNICallerPushLong(actionType);
    NJNICallerPushLong((int64_t)action);
    NJNICallerPushBoolean(needRelease);
    return NJNICallRetainObject(NULL, holdMethod);
}

void NPostOnMain(NAction *action) {
    static NJNIMethod *method = NULL;
    if (!method) {
        NJNIFinderReset();
        NJNIFinderAddArg(ValueClass());
        method = NJNIFindStaticMethod(RunLoopClass(), "postOnMain");
    }

    NJNIObject *actionObject = CreateJNIAction(action);
    {
        NJNICallerReset();
        NJNICallerPushObject(actionObject);
        NJNICallVoid(NULL, method);
    }
    NRelease(actionObject);
}
