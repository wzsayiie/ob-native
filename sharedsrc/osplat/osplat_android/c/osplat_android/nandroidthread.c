#include "nthread.h"
#include "cmeta.h"
#include "osbind.h"

//static NJNIClass *ThreadClass(void) {return NJNIImportClass("src/shared/osplat/NThread");}
//static NJNIClass *LongClass  (void) {return NJNIImportClass("long" );}
//static NJNIClass *FloatClass (void) {return NJNIImportClass("float");}

//mutex is not necessary here,
//cause the value obtained by each thread is the same.

void NRunThread(int64_t param, NThreadFunc func) {
    /*
    static NJNIMethod *method = NULL;
    if (!method) {
        NJNIFinderPrepare();
        NJNIFinderAddArg(LongClass());
        method = NJNIFindStaticMethod(ThreadClass(), "runThread");
    }

    int64_t lamWord = (int64_t)lambda;

    NJNICallerPrepareCall();
    NJNICallerPushLong(lamWord);
    NJNICallVoid(NULL, method);
    */
}

void NThreadSleep(float seconds) {
    /*
    static NJNIMethod *method = NULL;
    if (!method) {
        NJNIFinderPrepare();
        NJNIFinderAddArg(FloatClass());
        method = NJNIFindStaticMethod(ThreadClass(), "threadSleep");
    }

    NJNICallerPrepareCall();
    NJNICallerPushDouble(seconds);
    NJNICallVoid(NULL, method);
    */
}
