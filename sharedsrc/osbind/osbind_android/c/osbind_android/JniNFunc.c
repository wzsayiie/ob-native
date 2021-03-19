#include <jni.h>
#include "cmeta.h"

JNIEXPORT jint JNICALL
Java_src_shared_osbind_NFunc_findFuncByName(JNIEnv *env, jclass cls,
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
Java_src_shared_osbind_NFunc_funcReturnType(JNIEnv *env, jclass cls,
    jint index)
{
    return NFuncRetType(index);
}

JNIEXPORT jboolean JNICALL
Java_src_shared_osbind_NFunc_funcRetRetain(JNIEnv *env, jclass cls,
    jint index)
{
    return NFuncRetRetain(index);
}

JNIEXPORT jint JNICALL
Java_src_shared_osbind_NFunc_funcArgCount(JNIEnv *env, jclass cls,
    jint index)
{
    return NFuncArgCount(index);
}

JNIEXPORT jint JNICALL
Java_src_shared_osbind_NFunc_funcArgType(JNIEnv *env, jclass cls,
    jint index, int argIndex)
{
    return NFuncArgType(index, argIndex);
}

JNIEXPORT void JNICALL
Java_src_shared_osbind_NFunc_nativeFuncPrepare(JNIEnv *env, jclass cls)
{
    NFuncPrepare();
}

JNIEXPORT void JNICALL
Java_src_shared_osbind_NFunc_nativeFuncPushArg(JNIEnv *env, jclass cls,
    jint type, jlong word)
{
    NFuncPushArg(type, word);
}

JNIEXPORT jlong JNICALL
Java_src_shared_osbind_NFunc_nativeFuncCall(JNIEnv *env, jclass cls,
    jint index)
{
    return NFuncCall(index);
}
