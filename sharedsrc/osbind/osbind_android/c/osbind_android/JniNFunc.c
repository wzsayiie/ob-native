#include <jni.h>
#include "cmeta.h"

JNIEXPORT jint JNICALL
Java_src_shared_osbind_NFunc_findFunc(JNIEnv *env, jclass cls,
    jstring name)
{
    int fPos = 0;
    if (name) {
        const char *chars = (*env)->GetStringUTFChars(env, name, NULL);
        fPos = NFindFunc(chars);
        (*env)->ReleaseStringUTFChars(env, name, chars);
    }
    return fPos;
}

JNIEXPORT jint JNICALL
Java_src_shared_osbind_NFunc_funcRetType(JNIEnv *env, jclass cls,
    jint fPos)
{
    return NFuncRetType(fPos);
}

JNIEXPORT jboolean JNICALL
Java_src_shared_osbind_NFunc_funcRetRetained(JNIEnv *env, jclass cls,
    jint fPos)
{
    return NFuncRetRetained(fPos);
}

JNIEXPORT jlong JNICALL
Java_src_shared_osbind_NFunc_callFunc(JNIEnv *env, jclass cls,
    jint  fPos , jint  argc ,
    jint  type0, jint  type1, jint  type2, jint  type3,
    jlong word0, jlong word1, jlong word2, jlong word3)
{
    NType   types[] = {type0, type1, type2, type3};
    __NWord words[] = {word0, word1, word2, word3};

    return NCallFunc(fPos, argc, types, words);
}
