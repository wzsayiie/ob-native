#include <jni.h>
#include "cmeta.h"

JNIEXPORT jint JNICALL
Java_src_shared_osbind_NFunc_findFuncByName(JNIEnv *env, jclass cls,
    jstring fName)
{
    int fIndex = 0;
    if (fName) {
        const char *chars = (*env)->GetStringUTFChars(env, fName, NULL);
        fIndex = NFindFunc(chars);
        (*env)->ReleaseStringUTFChars(env, fName, chars);
    }
    return fIndex;
}

JNIEXPORT jint JNICALL
Java_src_shared_osbind_NFunc_funcReturnType(JNIEnv *env, jclass cls,
    jint fIndex)
{
    return NFuncRetType(fIndex);
}

JNIEXPORT jboolean JNICALL
Java_src_shared_osbind_NFunc_funcRetRetained(JNIEnv *env, jclass cls,
    jint fIndex)
{
    return NFuncRetRetained(fIndex);
}

JNIEXPORT jint JNICALL
Java_src_shared_osbind_NFunc_funcArgCount(JNIEnv *env, jclass cls,
    jint fIndex)
{
    return NFuncArgCount(fIndex);
}

JNIEXPORT jint JNICALL
Java_src_shared_osbind_NFunc_funcArgType(JNIEnv *env, jclass cls,
    jint fIndex, int aIndex)
{
    return NFuncArgType(fIndex, aIndex);
}

JNIEXPORT void JNICALL
Java_src_shared_osbind_NFunc_nativeCallerReset(JNIEnv *env, jclass cls)
{
    NCallerReset();
}

JNIEXPORT void JNICALL
Java_src_shared_osbind_NFunc_nativeCallerPush(JNIEnv *env, jclass cls,
    jint argType, jlong argWord)
{
    NCallerPush(argType, argWord);
}

JNIEXPORT jlong JNICALL
Java_src_shared_osbind_NFunc_nativeCall(JNIEnv *env, jclass cls,
    jint fIndex)
{
    return NCall(fIndex);
}
