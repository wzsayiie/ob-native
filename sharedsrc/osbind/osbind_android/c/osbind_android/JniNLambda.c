#include <jni.h>
#include "basis.h"

JNIEXPORT void JNICALL
Java_src_shared_osbind_NLambda_retainWord(__unused JNIEnv *env, __unused jclass cls,
    jlong word)
{
    NRef ref = (NRef)word;
    NRetain(ref);
}

JNIEXPORT void JNICALL
Java_src_shared_osbind_NLambda_releaseWord(__unused JNIEnv *env, __unused jclass cls,
    jlong word)
{
    NRef ref = (NRef)word;
    NRelease(ref);
}

JNIEXPORT void JNICALL
Java_src_shared_osbind_NLambda_prepare(__unused JNIEnv *env, __unused jclass cls,
    jlong lamWord)
{
    NLambda *lambda = (NLambda *)lamWord;
    NLambdaPrepareCall(lambda);
}

JNIEXPORT void JNICALL
Java_src_shared_osbind_NLambda_pushBool(__unused JNIEnv *env, __unused jclass cls,
    jlong lamWord, jboolean value)
{
    NLambda *lambda = (NLambda *)lamWord;
    NLambdaPushBool(lambda, value);
}

JNIEXPORT void JNICALL
Java_src_shared_osbind_NLambda_pushLong(__unused JNIEnv *env, __unused jclass cls,
    jlong lamWord, jlong value)
{
    NLambda *lambda = (NLambda *)lamWord;
    NLambdaPushInt64(lambda, value);
}

JNIEXPORT void JNICALL
Java_src_shared_osbind_NLambda_pushDouble(__unused JNIEnv *env, __unused jclass cls,
    jlong lamWord, jdouble value)
{
    NLambda *lambda = (NLambda *)lamWord;
    NLambdaPushDouble(lambda, value);
}

JNIEXPORT void JNICALL
Java_src_shared_osbind_NLambda_pushString(JNIEnv *env, __unused jclass cls,
    jlong lamWord, jstring value)
{
    NLambda *lambda = (NLambda *)lamWord;
    NString *string = NULL;
    if (value) {
        const char16_t *chars = (*env)->GetStringChars(env, value, NULL);
        string = NStringCreateWithUTFChars(NUTF16, chars);
        (*env)->ReleaseStringChars(env, value, chars);
    }

    NLambdaPushString(lambda, string);
}

JNIEXPORT void JNICALL
Java_src_shared_osbind_NLambda_pushObjWord(__unused JNIEnv *env, __unused jclass cls,
    jlong lamWord, jlong word)
{
    NLambda *lambda = (NLambda *)lamWord;
    NRef value = (NRef)word;

    NLambdaPushObject(lambda, value);
}

JNIEXPORT void JNICALL
Java_src_shared_osbind_NLambda_pushLamWord(__unused JNIEnv *env, __unused jclass cls,
    jlong lamWord, jlong word)
{
    NLambda *lambda = (NLambda *)lamWord;
    NLambda *value  = (NLambda *)word;

    NLambdaPushLambda(lambda, value);
}

JNIEXPORT void JNICALL
Java_src_shared_osbind_NLambda_invokeVoid(__unused JNIEnv *env, __unused jclass cls,
    jlong lamWord)
{
    NLambda *lambda = (NLambda *)lamWord;
    NLambdaCallVoid(lambda);
}

JNIEXPORT jboolean JNICALL
Java_src_shared_osbind_NLambda_invokeBool(__unused JNIEnv *env, __unused jclass cls,
    jlong lamWord)
{
    NLambda *lambda = (NLambda *)lamWord;
    return NLambdaCallBool(lambda);
}

JNIEXPORT jlong JNICALL
Java_src_shared_osbind_NLambda_invokeLong(__unused JNIEnv *env, __unused jclass cls,
    jlong lamWord)
{
    NLambda *lambda = (NLambda *)lamWord;
    return NLambdaCallInt64(lambda);
}

JNIEXPORT jdouble JNICALL
Java_src_shared_osbind_NLambda_invokeDouble(__unused JNIEnv *env, __unused jclass cls,
    jlong lamWord)
{
    NLambda *lambda = (NLambda *)lamWord;
    return NLambdaCallDouble(lambda);
}

JNIEXPORT jstring JNICALL
Java_src_shared_osbind_NLambda_invokeString(__unused JNIEnv *env, __unused jclass cls,
    jlong lamWord)
{
    NLambda *lambda = (NLambda *)lamWord;
    NString *retVal = NLambdaCallString(lambda);

    if (retVal) {
        const char16_t *chars = NStringU16Chars(retVal);
        if (chars) {
            jsize size = (jsize)NStringU16Size(retVal);

            //NOTE: surrogate pairs counted as 2 chars.
            jsize count = size / sizeof(char16_t);

            return (*env)->NewString(env, chars, count);
        }
    }
    return NULL;
}

JNIEXPORT jlong JNICALL
Java_src_shared_osbind_NLambda_invokeObjWord(__unused JNIEnv *env, __unused jclass cls,
    jlong lamWord)
{
    NLambda *lambda = (NLambda *)lamWord;
    NRef retVal = NLambdaCallObject(lambda);
    return (jlong)retVal;
}

JNIEXPORT jlong JNICALL
Java_src_shared_osbind_NLambda_invokeLamWord(__unused JNIEnv *env, __unused jclass cls,
    jlong lamWord)
{
    NLambda *lambda = (NLambda *)lamWord;
    NLambda *retVal = NLambdaCallLambda(lambda);
    return (jlong)retVal;
}
