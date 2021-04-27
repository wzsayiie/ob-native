#include <jni.h>
#include <string.h>
#include "basis.h"
#include "cmeta.h"

JNIEXPORT jlong JNICALL
Java_src_shared_osbind_NValue_floatWord(__unused JNIEnv *env, __unused jclass cls,
    jfloat value)
{
    jfloat buffer[2] = {value, 0.f};
    return *(jlong *)buffer;
}

JNIEXPORT jlong JNICALL
Java_src_shared_osbind_NValue_doubleWord(__unused JNIEnv *env, __unused jclass cls,
    jdouble value)
{
    return *(jlong *)&value;
}

JNIEXPORT jfloat JNICALL
Java_src_shared_osbind_NValue_floatValue(__unused JNIEnv *env, __unused jclass cls,
    jlong word)
{
    return *(jfloat *)&word;
}

JNIEXPORT jdouble JNICALL
Java_src_shared_osbind_NValue_doubleValue(__unused JNIEnv *env, __unused jclass cls,
    jlong word)
{
    return *(jdouble *)&word;
}

JNIEXPORT void JNICALL
Java_src_shared_osbind_NValue_retainWord(__unused JNIEnv *env, __unused jclass cls,
    jlong word)
{
    void *ptr = (void *)word;
    NRetain(ptr);
}

JNIEXPORT void JNICALL
Java_src_shared_osbind_NValue_releaseWord(__unused JNIEnv *env, __unused jclass cls,
    jlong word)
{
    void *ptr = (void *)word;
    NRelease(ptr);
}

JNIEXPORT jlong JNICALL
Java_src_shared_osbind_NValue_createStrWord(JNIEnv *env, __unused jclass cls,
    jstring string)
{
    NString *object = NULL;
    if (string) {
        const char16_t *chars = (*env)->GetStringChars(env, string, NULL);
        object = NStringCreateWithUTFChars(NUTF16, chars);
        (*env)->ReleaseStringChars(env, string, chars);
    }
    return (jlong)object;
}

JNIEXPORT jstring JNICALL
Java_src_shared_osbind_NValue_stringValue(JNIEnv *env, __unused jclass cls,
    jlong word)
{
    NString *object = (NString *)word;
    if (object) {
        const char16_t *chars = NStringU16Chars(object);
        if (chars) {
            jsize size = (jsize)NStringU16Size(object);

            //NOTE: surrogate pairs counted as 2 chars.
            jsize count = size / sizeof(char16_t);

            return (*env)->NewString(env, chars, count);
        }
    }
    return NULL;
}
