#include <jni.h>
#include "cmeta.h"

#define UNUSED(X) ((void) sizeof(X))

JNIEXPORT jint JNICALL
Java_src_shared_osbind_NFunc_findFuncByName(JNIEnv *env, jclass cls,
    jstring fName)
{
    UNUSED(cls);

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
    UNUSED(env);
    UNUSED(cls);

    return NFuncRetType(fIndex);
}

JNIEXPORT jboolean JNICALL
Java_src_shared_osbind_NFunc_funcRetRetained(JNIEnv *env, jclass cls,
    jint fIndex)
{
    UNUSED(env);
    UNUSED(cls);

    return NFuncRetRetained(fIndex);
}

JNIEXPORT jint JNICALL
Java_src_shared_osbind_NFunc_funcArgCount(JNIEnv *env, jclass cls,
    jint fIndex)
{
    UNUSED(env);
    UNUSED(cls);

    return NFuncArgCount(fIndex);
}

JNIEXPORT jint JNICALL
Java_src_shared_osbind_NFunc_funcArgType(JNIEnv *env, jclass cls,
    jint fIndex, int aIndex)
{
    UNUSED(env);
    UNUSED(cls);

    return NFuncArgType(fIndex, aIndex);
}

JNIEXPORT jlong JNICALL
Java_src_shared_osbind_NFunc_invokeFunc(JNIEnv *env, jclass cls,
    jint  fIndex, jint  argc ,
    jint  type0 , jint  type1, jint  type2, jint  type3,
    jlong word0 , jlong word1, jlong word2, jlong word3)
{
    UNUSED(env);
    UNUSED(cls);

    NType   types[] = {type0, type1, type2, type3};
    __NWord words[] = {word0, word1, word2, word3};

    return NCallFunc(fIndex, argc, types, words);
}
