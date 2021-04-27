#include <jni.h>
#include "cmeta.h"

JNIEXPORT jint JNICALL
Java_src_shared_osbind_NFunc_findFuncIndex(JNIEnv *env, __unused jclass cls,
    jstring name)
{
    int index = 0;
    if (name) {
        const char *chars = (*env)->GetStringUTFChars(env, name, NULL);
        index = NFindFunc(chars);
        (*env)->ReleaseStringUTFChars(env, name, chars);
    }
    return index;
}

JNIEXPORT jint JNICALL
Java_src_shared_osbind_NFunc_funcRetType(__unused JNIEnv *env, __unused jclass cls,
    jint index)
{
    return NFuncRetType(index);
}

JNIEXPORT jboolean JNICALL
Java_src_shared_osbind_NFunc_funcRetRetain(__unused JNIEnv *env, __unused jclass cls,
    jint index)
{
    return NFuncRetRetain(index);
}

JNIEXPORT void JNICALL
Java_src_shared_osbind_NFunc_funcPrepare(__unused JNIEnv *env, __unused jclass cls)
{
    NFuncPrepare();
}

JNIEXPORT void JNICALL
Java_src_shared_osbind_NFunc_funcPushArg(__unused JNIEnv *env, __unused jclass cls,
    jint type, jlong word)
{
    NFuncPushArg(type, word);
}

JNIEXPORT jlong JNICALL
Java_src_shared_osbind_NFunc_funcCall(__unused JNIEnv *env, __unused jclass cls,
    jint index)
{
    return NFuncCall(index);
}
