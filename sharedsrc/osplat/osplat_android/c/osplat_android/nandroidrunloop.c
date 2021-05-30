#include "nrunloop.h"
#include "cmeta.h"
#include "osbind.h"

//static NJNIClass *RunLoopClass(void) {return NJNIImportClass("src/shared/osplat/NRunLoop");}
//static NJNIClass *LongClass   (void) {return NJNIImportClass("long");}

//mutex is not necessary here,
//cause the value obtained by each thread is the same.

void NPostTaskOnMain(int64_t param, NTaskFunc func) {
    /*
    static NJNIMethod *method = NULL;
    if (!method) {
        NJNIFinderPrepare();
        NJNIFinderAddArg(LongClass());
        method = NJNIFindStaticMethod(RunLoopClass(), "postOnMain");
    }

    int64_t word = (int64_t)lambda;

    NJNICallerPrepareCall();
    NJNICallerPushLong(word);
    NJNICallVoid(NULL, method);
    */
}
