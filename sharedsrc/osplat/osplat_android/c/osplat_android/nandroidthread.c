#include "nthread.h"
#include "cmeta.h"
#include "osbind.h"

static NJNIClass *ThreadClass (void) {return NJNIImportClass("src/shared/osplat/NThread");}
static NJNIClass *ValueClass  (void) {return NJNIImportClass("src/shared/osbind/NValue" );}
static NJNIClass *IntClass    (void) {return NJNIImportClass("int"    );}
static NJNIClass *LongClass   (void) {return NJNIImportClass("long"   );}
static NJNIClass *BooleanClass(void) {return NJNIImportClass("boolean");}
static NJNIClass *FloatClass  (void) {return NJNIImportClass("float"  );}

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

void NRunThread(NAction *action) {
    static NJNIMethod *method = NULL;
    if (!method) {
        NJNIFinderReset();
        NJNIFinderAddArg(ValueClass());
        method = NJNIFindStaticMethod(ThreadClass(), "runThread");
    }

    NJNIObject *actionObject = CreateJNIAction(action);
    {
        NJNICallerReset();
        NJNICallerPushObject(actionObject);
        NJNICallVoid(NULL, method);
    }
    NRelease(actionObject);
}

void NThreadSleep(float seconds) {
    static NJNIMethod *method = NULL;
    if (!method) {
        NJNIFinderReset();
        NJNIFinderAddArg(FloatClass());
        method = NJNIFindStaticMethod(ThreadClass(), "threadSleep");
    }

    NJNICallerReset();
    NJNICallerPushDouble(seconds);
    NJNICallVoid(NULL, method);
}
