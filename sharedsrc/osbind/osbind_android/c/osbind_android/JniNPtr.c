#include <jni.h>
#include "basis.h"

JNIEXPORT jlong JNICALL
Java_src_shared_osbind_NPtr_createString(JNIEnv *env, jclass cls,
    jstring string)
{
    NString *object = NULL;
    if (string) {
        const jchar *chars = (*env)->GetStringChars(env, string, NULL);
        object = NStringCreateWithUTFChars(NUTF16, chars);
        (*env)->ReleaseStringChars(env, string, chars);
    }
    return NMakePtrWord(object);
}

JNIEXPORT jstring JNICALL
Java_src_shared_osbind_NPtr_stringFromPtr(JNIEnv *env, jclass cls,
    jlong ptr)
{
    if (!ptr) {
        return NULL;
    }

    NString *object = NPtrWord(ptr);
    const char16_t *chars = NStringU16Chars(object);
    int length = NStringLength(object);

    if (chars) {
        return (*env)->NewString(env, chars, (jsize)length);
    }
    return NULL;
}

JNIEXPORT jstring JNICALL
Java_src_shared_osbind_NPtr_charsFromPtr(JNIEnv *env, jclass cls,
    jlong ptr)
{
    if (ptr) {
        const char *object = NPtrWord(ptr);
        return (*env)->NewStringUTF(env, object);
    }
    return NULL;
}

JNIEXPORT void JNICALL
Java_src_shared_osbind_NPtr_releasePtr(JNIEnv *env, jclass cls,
    jlong ptr)
{
    void *object = NPtrWord(ptr);
    NRelease(object);
}
