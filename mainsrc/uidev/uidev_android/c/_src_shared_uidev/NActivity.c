#include <jni.h>
#include "uidev.h"

JNIEXPORT void JNICALL
Java_src_shared_uidev_NActivity_handleCreate(JNIEnv *env, jclass cls) {
    NLaunch();
}
