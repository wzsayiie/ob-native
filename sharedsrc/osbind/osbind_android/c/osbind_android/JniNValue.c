#include <jni.h>
#include <string.h>
#include "basis.h"
#include "cmeta.h"

#define UNUSED(X) ((void) sizeof(X))

JNIEXPORT jlong JNICALL
Java_src_shared_osbind_NValue_makeFloatWord(JNIEnv *env, jclass cls,
    jfloat value)
{
    UNUSED(env);
    UNUSED(cls);

    jfloat buffer[2] = {value, 0.f};
    return *(jlong *)buffer;
}

JNIEXPORT jlong JNICALL
Java_src_shared_osbind_NValue_makeDoubleWord(JNIEnv *env, jclass cls,
    jdouble value)
{
    UNUSED(env);
    UNUSED(cls);

    return *(jlong *)&value;
}

JNIEXPORT jfloat JNICALL
Java_src_shared_osbind_NValue_floatValue(JNIEnv *env, jclass cls,
    jlong word)
{
    UNUSED(env);
    UNUSED(cls);

    return *(jfloat *)&word;
}

JNIEXPORT jdouble JNICALL
Java_src_shared_osbind_NValue_doubleValue(JNIEnv *env, jclass cls,
    jlong word)
{
    UNUSED(env);
    UNUSED(cls);

    return *(jdouble *)&word;
}

JNIEXPORT jint JNICALL
Java_src_shared_osbind_NValue_nativeStringType(JNIEnv *env, jclass cls) {
    UNUSED(env);
    UNUSED(cls);

    static int type = 0;
    //mutex is not necessary here, cause the value obtained by each thread is the same.
    if (type == 0) {
        type = NFindStruct("NString");
    }
    return type;
}

JNIEXPORT jlong JNICALL
Java_src_shared_osbind_NValue_createString(JNIEnv *env, jclass cls,
    jstring string)
{
    UNUSED(cls);

    NString *object = NULL;
    if (string) {
        const char16_t *chars = (*env)->GetStringChars(env, string, NULL);
        object = NStringCreateWithUTFChars(NUTF16, chars);
        (*env)->ReleaseStringChars(env, string, chars);
    }
    return (jlong)object;
}

JNIEXPORT jlong JNICALL
Java_src_shared_osbind_NValue_allocU8Chars(JNIEnv *env, jclass cls,
    jstring string)
{
    UNUSED(cls);

    char *object = NULL;
    if (string) {
        const char *chars = (*env)->GetStringUTFChars(env, string, NULL);

        int count = (int)strlen(chars);
        int size = (count + 1) * nisizeof(char);
        object = NAlloc(size);
        NMoveMemory(object, chars, size);

        (*env)->ReleaseStringUTFChars(env, string, chars);
    }
    return (jlong)object;
}

static size_t strlen16(const char16_t *src) {
    size_t count = 0;
    for (const char16_t *ptr = src; *ptr; ++ptr) {
        count += 1;
    }
    return count;
}

JNIEXPORT jlong JNICALL
Java_src_shared_osbind_NValue_allocU16Chars(JNIEnv *env, jclass cls,
    jstring string)
{
    UNUSED(cls);

    char16_t *object = NULL;
    if (string) {
        const char16_t *chars = (*env)->GetStringChars(env, string, NULL);

        int count = (int)strlen16(chars);
        int size = (count + 1) * nisizeof(char16_t);
        object = NAlloc(size);
        NMoveMemory(object, chars, size);

        (*env)->ReleaseStringChars(env, string, chars);
    }
    return (jlong)object;
}

JNIEXPORT void JNICALL
Java_src_shared_osbind_NValue_releaseObject(JNIEnv *env, jclass cls,
    jlong word)
{
    UNUSED(env);
    UNUSED(cls);

    void *ptr = (void *)word;
    NRelease(ptr);
}

JNIEXPORT void JNICALL
Java_src_shared_osbind_NValue_freeMemory(JNIEnv *env, jclass cls,
    jlong word)
{
    UNUSED(env);
    UNUSED(cls);

    void *ptr = (void *)word;
    NFree(ptr);
}

JNIEXPORT jstring JNICALL
Java_src_shared_osbind_NValue_castString(JNIEnv *env, jclass cls,
    jlong word)
{
    UNUSED(cls);

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

JNIEXPORT jstring JNICALL
Java_src_shared_osbind_NValue_castU8Chars(JNIEnv *env, jclass cls,
    jlong word)
{
    UNUSED(cls);

    const char *chars = (const char *)word;
    if (chars) {
        return (*env)->NewStringUTF(env, chars);
    }
    return NULL;
}

JNIEXPORT jstring JNICALL
Java_src_shared_osbind_NValue_castU16Chars(JNIEnv *env, jclass cls,
    jlong word)
{
    UNUSED(cls);

    const char16_t *chars = (const char16_t *)word;
    if (chars) {
        jsize size  = (jsize)strlen16(chars);
        jsize count = size / sizeof(char16_t);
        return (*env)->NewString(env, chars, count);
    }
    return NULL;
}
