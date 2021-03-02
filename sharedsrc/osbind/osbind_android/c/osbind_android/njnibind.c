#include "njnibind.h"
#include "binlist.h"
#include "cstr.h"

static const jint JNI_VERSION = JNI_VERSION_1_6;
static JavaVM *sJNIVM = NULL;

jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    sJNIVM = vm;
    return JNI_VERSION;
}

static JNIEnv *GetEnv(void) {
    JNIEnv *env = NULL;
    (*sJNIVM)->GetEnv(sJNIVM, (void **)&env, JNI_VERSION);
    return env;
}

static void JNIClassInit(NJNIClass *cls) {
    _NObjectInit(nsuperof(cls));
}

static void JNIClassDeinit(NJNIClass *cls) {
    //the class name.
    mfree(cls->className);
    //the jni reference.
    if (cls->jniRef) {
        JNIEnv *env = GetEnv();
        (*env)->DeleteGlobalRef(env, cls->jniRef);
    }
    //the item class.
    NRelease(cls->itemClass);

    _NObjectDeinit(nsuperof(cls));
}

static NJNIClass *JNIClassCreate(void) {
    NJNIClass *cls = NAlloc(NJNIClass, JNIClassDeinit);
    JNIClassInit(cls);
    return cls;
}

static int JNIClassComparer(word x, word y) {
    NJNIClass *a = x.asptr;
    NJNIClass *b = y.asptr;

    //compare the pointers.
    if (!a || !b) {
        if (a) {return  1;}
        if (b) {return -1;}
        else   {return  0;}
    }

    //compare the types.
    if (a->classType != b->classType) {
        return (a->classType) > (b->classType) ? 1 : -1;
    }

    //compare the names.
    if (a->className && b->className) {
        int result = cscmp(a->className, b->className);
        if (result != 0) {
            return result;
        }
    } else {
        if (a->className) {return  1;}
        if (b->className) {return -1;}
    }

    //compare the item classes.
    return JNIClassComparer(pw(a->itemClass), pw(b->itemClass));
}

static char *DupClassName(const char *src) {
    char *dst = csdup(src);
    //the class name format "xx.xx.xx" and "xx/xx/xx" are both available.
    for (char *i = dst; *i; ++i) {
        if (*i == '.') {
            *i = '/';
        }
    }
    return dst;
}

static NJNIClass *ImportClass(_NJNIClassType classType, const char *className, NJNIClass *itemClass) {
    static binlist list = {0};
    nsynonce() {
        blinit(&list, JNIClassComparer);
    }

    NJNIClass key = {0};
    key.classType = classType;
    key.className = (char *)className;
    key.itemClass = itemClass;

    NJNIClass *cls = NULL;
    nsyn() {
        //if the class already exists.
        cls = blvalue(&list, pw(&key)).asptr;
        if (cls) {
            return cls;
        }

        //import a class type.
        if (classType == _NJNI_CLASS_OBJECT) {
            JNIEnv *env = GetEnv();
            jclass localRef = (*env)->FindClass(env, className);

            if (localRef) {
                jclass globalRef = (*env)->NewGlobalRef(env, localRef);
                (*env)->DeleteLocalRef(env, localRef);

                cls = JNIClassCreate();
                cls->classType = classType;
                cls->className = csdup(className);
                cls->jniRef    = globalRef;
                cls->itemClass = NRetain(itemClass);
            }
        }
        //import a array type.
        else if (classType == _NJNI_CLASS_ARRAY) {
            cls = JNIClassCreate();
            cls->classType = classType;
            cls->itemClass = NRetain(itemClass);
        }
        //import a basic type.
        else {
            cls = JNIClassCreate();
            cls->classType = classType;
        }

        if (cls) {
            blinsert(&list, pw(cls), pw(cls));
        }
    }
    return cls;
}

NJNIClass *NJNIImportClass(const char *name) {
    if (!name || *name == '\0') {
        return NULL;
    }

    _NJNIClassType type = _NJNI_CLASS_OBJECT;
    if /**/ (cscmp(name, "void"   ) == 0) {type = _NJNI_CLASS_VOID   ;}
    else if (cscmp(name, "boolean") == 0) {type = _NJNI_CLASS_BOOLEAN;}
    else if (cscmp(name, "char"   ) == 0) {type = _NJNI_CLASS_CHAR   ;}
    else if (cscmp(name, "byte"   ) == 0) {type = _NJNI_CLASS_BYTE   ;}
    else if (cscmp(name, "short"  ) == 0) {type = _NJNI_CLASS_SHORT  ;}
    else if (cscmp(name, "int"    ) == 0) {type = _NJNI_CLASS_INT    ;}
    else if (cscmp(name, "long"   ) == 0) {type = _NJNI_CLASS_LONG   ;}
    else if (cscmp(name, "float"  ) == 0) {type = _NJNI_CLASS_FLOAT  ;}
    else if (cscmp(name, "double" ) == 0) {type = _NJNI_CLASS_DOUBLE ;}

    return ImportClass(type, name, NULL);
}

NJNIClass *NJNIImportArrayClass(NJNIClass *itemClass) {
    if (!itemClass) {
        return NULL;
    }
    if (itemClass->classType == _NJNI_CLASS_VOID) {
        return NULL;
    }

    return ImportClass(_NJNI_CLASS_ARRAY, NULL, itemClass);
}

static void JNIObjectInit(NJNIObject *object) {
    _NObjectInit(nsuperof(object));
}

static void JNIObjectDeinit(NJNIObject *object) {
    //the class reference.
    NRelease(object->cls);
    //the jni reference.
    if (object->jniRef) {
        JNIEnv *env = GetEnv();
        (*env)->DeleteGlobalRef(env, object->jniRef);
    }

    _NObjectDeinit(nsuperof(object));
}

static NJNIObject *JNIObjectCreate(void) {
    NJNIObject *object = NAlloc(NJNIObject, JNIObjectDeinit);
    JNIObjectInit(object);
    return object;
}

NJNIObject *NJNICreateArray(NJNIClass *cls, int count) {
    if (!cls) {
        return NULL;
    }
    if (cls->classType != _NJNI_CLASS_ARRAY) {
        return NULL;
    }

    JNIEnv *env = GetEnv();
    jobject localRef = NULL;
    switch (cls->itemClass->classType) {
        case _NJNI_CLASS_BOOLEAN: localRef = (*env)->NewBooleanArray(env, count); break;
        case _NJNI_CLASS_CHAR   : localRef = (*env)->NewCharArray   (env, count); break;
        case _NJNI_CLASS_BYTE   : localRef = (*env)->NewByteArray   (env, count); break;
        case _NJNI_CLASS_SHORT  : localRef = (*env)->NewShortArray  (env, count); break;
        case _NJNI_CLASS_INT    : localRef = (*env)->NewIntArray    (env, count); break;
        case _NJNI_CLASS_LONG   : localRef = (*env)->NewLongArray   (env, count); break;
        case _NJNI_CLASS_FLOAT  : localRef = (*env)->NewFloatArray  (env, count); break;
        case _NJNI_CLASS_DOUBLE : localRef = (*env)->NewDoubleArray (env, count); break;

        default/*STR, OBJ, ARR*/: {
            localRef = (*env)->NewObjectArray(env, count, cls->itemClass->jniRef, NULL);
        }
    }
    if (!localRef) {
        return NULL;
    }

    jobject globalRef = (*env)->NewGlobalRef(env, localRef);
    (*env)->DeleteLocalRef(env, localRef);

    NJNIObject *array = JNIObjectCreate();
    array->cls = NRetain(cls);
    array->jniRef = globalRef;
    return array;
}

static bool IsArrayObject(NJNIObject *object) {
    if (!object) {
        return false;
    }
    if (object->cls->classType != _NJNI_CLASS_ARRAY) {
        return false;
    }

    return true;
}

int NJNIArrayCount(NJNIObject *array) {
    if (IsArrayObject(array)) {
        JNIEnv *env = GetEnv();
        return (*env)->GetArrayLength(env, array->jniRef);
    }
    return 0;
}

void NJNISetBooleanItem(NJNIObject *array, int idx, bool itm) {
    if (!IsArrayObject(array)) {
        return;
    }

    jobject ref = array->jniRef;
    JNIEnv *env = GetEnv();
    switch (array->cls->itemClass->classType) {
        case _NJNI_CLASS_BOOLEAN: {jboolean v = itm; (*env)->SetBooleanArrayRegion(env, ref, idx, 1, &v); break;}
        case _NJNI_CLASS_BYTE   : {jbyte    v = itm; (*env)->SetByteArrayRegion   (env, ref, idx, 1, &v); break;}
        case _NJNI_CLASS_SHORT  : {jshort   v = itm; (*env)->SetShortArrayRegion  (env, ref, idx, 1, &v); break;}
        case _NJNI_CLASS_INT    : {jint     v = itm; (*env)->SetIntArrayRegion    (env, ref, idx, 1, &v); break;}
        case _NJNI_CLASS_LONG   : {jlong    v = itm; (*env)->SetLongArrayRegion   (env, ref, idx, 1, &v); break;}
        case _NJNI_CLASS_FLOAT  : {jfloat   v = itm; (*env)->SetFloatArrayRegion  (env, ref, idx, 1, &v); break;}
        case _NJNI_CLASS_DOUBLE : {jdouble  v = itm; (*env)->SetDoubleArrayRegion (env, ref, idx, 1, &v); break;}
        default/*CHR,S,OBJ,ARR*/: ;
    }
}

void NJNISetLongItem(NJNIObject *array, int idx, int64_t itm) {
    if (!IsArrayObject(array)) {
        return;
    }

    jobject ref = array->jniRef;
    JNIEnv *env = GetEnv();
    switch (array->cls->itemClass->classType) {
        case _NJNI_CLASS_BOOLEAN: {jboolean v = itm ? 1 : 0; (*env)->SetBooleanArrayRegion(env, ref, idx, 1, &v); break;}
        case _NJNI_CLASS_CHAR   : {jchar    v = (jchar )itm; (*env)->SetCharArrayRegion   (env, ref, idx, 1, &v); break;}
        case _NJNI_CLASS_BYTE   : {jbyte    v = (jbyte )itm; (*env)->SetByteArrayRegion   (env, ref, idx, 1, &v); break;}
        case _NJNI_CLASS_SHORT  : {jshort   v = (jshort)itm; (*env)->SetShortArrayRegion  (env, ref, idx, 1, &v); break;}
        case _NJNI_CLASS_INT    : {jint     v = (jint  )itm; (*env)->SetIntArrayRegion    (env, ref, idx, 1, &v); break;}
        case _NJNI_CLASS_LONG   : {jlong    v = /*long*/itm; (*env)->SetLongArrayRegion   (env, ref, idx, 1, &v); break;}
        case _NJNI_CLASS_FLOAT  : {jfloat   v = /*flt */itm; (*env)->SetFloatArrayRegion  (env, ref, idx, 1, &v); break;}
        case _NJNI_CLASS_DOUBLE : {jdouble  v = /*dbl */itm; (*env)->SetDoubleArrayRegion (env, ref, idx, 1, &v); break;}
        default/*STR, OBJ, ARR*/: ;
    }
}

void NJNISetDoubleItem(NJNIObject *array, int idx, double itm) {
    if (!IsArrayObject(array)) {
        return;
    }

    jobject ref = array->jniRef;
    JNIEnv *env = GetEnv();
    switch (array->cls->itemClass->classType) {
        case _NJNI_CLASS_BOOLEAN: {jboolean v = itm ? 1 : 0; (*env)->SetBooleanArrayRegion(env, ref, idx, 1, &v); break;}
        case _NJNI_CLASS_BYTE   : {jbyte    v = (jbyte )itm; (*env)->SetByteArrayRegion   (env, ref, idx, 1, &v); break;}
        case _NJNI_CLASS_SHORT  : {jshort   v = (jshort)itm; (*env)->SetShortArrayRegion  (env, ref, idx, 1, &v); break;}
        case _NJNI_CLASS_INT    : {jint     v = (jint  )itm; (*env)->SetIntArrayRegion    (env, ref, idx, 1, &v); break;}
        case _NJNI_CLASS_LONG   : {jlong    v = (jlong )itm; (*env)->SetLongArrayRegion   (env, ref, idx, 1, &v); break;}
        case _NJNI_CLASS_FLOAT  : {jfloat   v = (jfloat)itm; (*env)->SetFloatArrayRegion  (env, ref, idx, 1, &v); break;}
        case _NJNI_CLASS_DOUBLE : {jdouble  v = /*dbl */itm; (*env)->SetDoubleArrayRegion (env, ref, idx, 1, &v); break;}
        default/*CHR,S,OBJ,ARR*/: ;
    }
}

static jstring LocalJNIString(JNIEnv *env, NString *string) {
    jstring jniRef = NULL;
    if (!NStringIsEmpty(string)) {
        const char16_t *chars = NStringU16Chars(string);
        //NOTE: a surrogate pair count as two chars.
        jsize length = NStringU16Size(string) / nsizeof(char16_t);

        jniRef = (*env)->NewString(env, chars, length);
    }
    return jniRef;
}

static NString *CreateStringFromJNI(JNIEnv *env, jstring jniRef) {
    NString *string = NULL;
    if (jniRef) {
        //NOTE: the chars buffer doesn't end with '\0'.
        const char16_t *chars = (*env)->GetStringChars(env, jniRef, NULL);
        int length = (*env)->GetStringLength(env, jniRef);

        string = NStringCreateWithUTFBytes(NUTF16, chars, chars + length);
        (*env)->ReleaseStringChars(env, jniRef, chars);
    }
    return string;
}

void NJNISetStringItem(NJNIObject *array, int index, NString *item) {
    if (!IsArrayObject(array)) {
        return;
    }

    JNIEnv *env = GetEnv();
    jstring jniRef = LocalJNIString(env, item);
    (*env)->SetObjectArrayElement(env, array->jniRef, index, jniRef);
    (*env)->DeleteLocalRef(env, jniRef);
}

void NJNISetObjectItem(NJNIObject *array, int index, NJNIObject *item) {
    if (!IsArrayObject(array)) {
        return;
    }

    JNIEnv *env = GetEnv();
    (*env)->SetObjectArrayElement(env, array->jniRef, index, item->jniRef);
}

bool NJNIBooleanItem(NJNIObject *array, int idx) {
    if (!IsArrayObject(array)) {
        return false;
    }

    jobject ref = array->jniRef;
    JNIEnv *env = GetEnv();
    switch (array->cls->itemClass->classType) {
        case _NJNI_CLASS_BOOLEAN: {jboolean v = 0; (*env)->GetBooleanArrayRegion(env, ref, idx, 1, &v); return (bool)v;}
        case _NJNI_CLASS_CHAR   : {jchar    v = 0; (*env)->GetCharArrayRegion   (env, ref, idx, 1, &v); return (bool)v;}
        case _NJNI_CLASS_BYTE   : {jbyte    v = 0; (*env)->GetByteArrayRegion   (env, ref, idx, 1, &v); return (bool)v;}
        case _NJNI_CLASS_SHORT  : {jshort   v = 0; (*env)->GetShortArrayRegion  (env, ref, idx, 1, &v); return (bool)v;}
        case _NJNI_CLASS_INT    : {jint     v = 0; (*env)->GetIntArrayRegion    (env, ref, idx, 1, &v); return (bool)v;}
        case _NJNI_CLASS_LONG   : {jlong    v = 0; (*env)->GetLongArrayRegion   (env, ref, idx, 1, &v); return (bool)v;}
        case _NJNI_CLASS_FLOAT  : {jfloat   v = 0; (*env)->GetFloatArrayRegion  (env, ref, idx, 1, &v); return (bool)v;}
        case _NJNI_CLASS_DOUBLE : {jdouble  v = 0; (*env)->GetDoubleArrayRegion (env, ref, idx, 1, &v); return (bool)v;}

        default/*STR, OBJ, ARR*/: {
            jobject object = (*env)->GetObjectArrayElement(env, ref, idx);
            (*env)->DeleteLocalRef(env, object);
            return (bool)object;
        }
    }
}

int64_t NJNILongItem(NJNIObject *array, int idx) {
    if (!IsArrayObject(array)) {
        return 0;
    }

    jobject ref = array->jniRef;
    JNIEnv *env = GetEnv();
    switch (array->cls->itemClass->classType) {
        case _NJNI_CLASS_BOOLEAN: {jboolean v = 0; (*env)->GetBooleanArrayRegion(env, ref, idx, 1, &v); return /*int64*/v;}
        case _NJNI_CLASS_CHAR   : {jchar    v = 0; (*env)->GetCharArrayRegion   (env, ref, idx, 1, &v); return /*int64*/v;}
        case _NJNI_CLASS_BYTE   : {jbyte    v = 0; (*env)->GetByteArrayRegion   (env, ref, idx, 1, &v); return /*int64*/v;}
        case _NJNI_CLASS_SHORT  : {jshort   v = 0; (*env)->GetShortArrayRegion  (env, ref, idx, 1, &v); return /*int64*/v;}
        case _NJNI_CLASS_INT    : {jint     v = 0; (*env)->GetIntArrayRegion    (env, ref, idx, 1, &v); return /*int64*/v;}
        case _NJNI_CLASS_LONG   : {jlong    v = 0; (*env)->GetLongArrayRegion   (env, ref, idx, 1, &v); return /*int64*/v;}
        case _NJNI_CLASS_FLOAT  : {jfloat   v = 0; (*env)->GetFloatArrayRegion  (env, ref, idx, 1, &v); return (int64_t)v;}
        case _NJNI_CLASS_DOUBLE : {jdouble  v = 0; (*env)->GetDoubleArrayRegion (env, ref, idx, 1, &v); return (int64_t)v;}

        default/*STR, OBJ, ARR*/: {
            return 0;
        }
    }
}

double NJNIDoubleItem(NJNIObject *array, int idx) {
    if (!IsArrayObject(array)) {
        return 0;
    }

    jobject ref = array->jniRef;
    JNIEnv *env = GetEnv();
    switch (array->cls->itemClass->classType) {
        case _NJNI_CLASS_BOOLEAN: {jboolean v = 0; (*env)->GetBooleanArrayRegion(env, ref, idx, 1, &v); return v;}
        case _NJNI_CLASS_BYTE   : {jbyte    v = 0; (*env)->GetByteArrayRegion   (env, ref, idx, 1, &v); return v;}
        case _NJNI_CLASS_SHORT  : {jshort   v = 0; (*env)->GetShortArrayRegion  (env, ref, idx, 1, &v); return v;}
        case _NJNI_CLASS_INT    : {jint     v = 0; (*env)->GetIntArrayRegion    (env, ref, idx, 1, &v); return v;}
        case _NJNI_CLASS_LONG   : {jlong    v = 0; (*env)->GetLongArrayRegion   (env, ref, idx, 1, &v); return v;}
        case _NJNI_CLASS_FLOAT  : {jfloat   v = 0; (*env)->GetFloatArrayRegion  (env, ref, idx, 1, &v); return v;}
        case _NJNI_CLASS_DOUBLE : {jdouble  v = 0; (*env)->GetDoubleArrayRegion (env, ref, idx, 1, &v); return v;}

        default/*CHR,S,OBJ,ARR*/: {
            return 0.0;
        }
    }
}

NString *NJNIRetainStringItem(NJNIObject *array, int index) {
    if (!IsArrayObject(array)) {
        return NULL;
    }

    JNIEnv *env = GetEnv();
    jstring jniRef = (*env)->GetObjectArrayElement(env, array->jniRef, index);
    if (!jniRef) {
        return NULL;
    }

    NString *string = CreateStringFromJNI(env, jniRef);
    (*env)->DeleteLocalRef(env, jniRef);
    return string;
}

NJNIObject *NJNIRetainObjectItem(NJNIObject *array, int index) {
    if (!IsArrayObject(array)) {
        return NULL;
    }

    JNIEnv *env = GetEnv();
    jobject localRef = (*env)->GetObjectArrayElement(env, array->jniRef, index);
    if (!localRef) {
        return NULL;
    }

    jobject globalRef = (*env)->NewGlobalRef(env, localRef);
    (*env)->DeleteLocalRef(env, localRef);

    NJNIObject *object = JNIObjectCreate();
    object->cls = NRetain(array->cls->itemClass);
    object->jniRef = globalRef;
    return object;
}

static void JNIMethodInit(NJNIMethod *method) {
    _NObjectInit(nsuperof(method));
}

static void JNIMethodDeinit(NJNIMethod *method) {
    //the class reference.
    NRelease(method->owningClass);
    //the method name.
    mfree(method->methodName);
    //the method signature.
    mfree(method->signature);
    //the return class type.
    NRelease(method->retClass);
    //the argument class types.
    for (int i = 0; i < method->argCount; ++i) {
        NRelease(method->argClasses[i]);
    }

    _NObjectDeinit(nsuperof(method));
}

static NJNIMethod *JNIMethodCreate(void) {
    NJNIMethod *method = NAlloc(NJNIMethod, JNIMethodDeinit);
    JNIMethodInit(method);
    return method;
}

static nthreadlocal NJNIClass *sFinderRetClass = NULL;
static nthreadlocal int        sFinderArgCount = 0;
static nthreadlocal NJNIClass *sFinderArgClasses[_NJNI_MAX_ARG_NUM] = {NULL};

void NJNIFinderReset(void) {
    sFinderRetClass = NULL;
    sFinderArgCount = 0;
}

void NJNIFinderSetRet(NJNIClass *ret) {
    sFinderRetClass = ret;
}

void NJNIFinderAddArg(NJNIClass *arg) {
    if (sFinderArgCount < _NJNI_MAX_ARG_NUM) {
        sFinderArgClasses[sFinderArgCount] = arg;
        sFinderArgCount += 1;
    }
}

static int JNIMethodComparer(word x, word y) {
    NJNIMethod *a = x.asptr;
    NJNIMethod *b = y.asptr;

    //compare classes.
    if (a->owningClass != b->owningClass) {
        return (a->owningClass) > (b->owningClass) ? 1 : -1;
    }

    //compare types.
    if (a->methodType != b->methodType) {
        return (a->methodType) > (b->methodType) ? 1 : -1;
    }

    //compare names.
    if (a->methodName && b->methodName) {
        int result = cscmp(a->methodName, b->methodName);
        if (result != 0) {
            return result;
        }
    } else {
        if (a->methodName) {return  1;}
        if (b->methodName) {return -1;}
    }

    //compare signatures.
    if (a->signature && b->signature) {
        int result = cscmp(a->signature, b->signature);
        if (result != 0) {
            return result;
        }
    } else {
        if (a->signature) {return  1;}
        if (b->signature) {return -1;}
    }

    return 0;
}

static void ConcatenateSignature(char *buffer, NJNIClass *cls) {
    if (!cls || cls->classType == _NJNI_CLASS_VOID) {
        cscat(buffer, "V");
        return;
    }

    switch (cls->classType) {
        case _NJNI_CLASS_BOOLEAN: cscat(buffer, "Z"); return;
        case _NJNI_CLASS_CHAR   : cscat(buffer, "C"); return;
        case _NJNI_CLASS_BYTE   : cscat(buffer, "B"); return;
        case _NJNI_CLASS_SHORT  : cscat(buffer, "S"); return;
        case _NJNI_CLASS_INT    : cscat(buffer, "I"); return;
        case _NJNI_CLASS_LONG   : cscat(buffer, "J"); return;
        case _NJNI_CLASS_FLOAT  : cscat(buffer, "F"); return;
        case _NJNI_CLASS_DOUBLE : cscat(buffer, "D"); return;
        default:;
    }

    if (cls->classType == _NJNI_CLASS_OBJECT) {
        cscat(buffer, "L");
        cscat(buffer, cls->className);
        cscat(buffer, ";");
        return;
    }

    if (cls->classType == _NJNI_CLASS_ARRAY) {
        cscat(buffer, "[");
        ConcatenateSignature(buffer, cls->itemClass);
        return;
    }
}

static NJNIMethod *FindMethod(NJNIClass *cls, _NJNIMethodType type, const char *name) {
    static binlist list = {0};
    nsynonce() {
        blinit(&list, JNIMethodComparer);
    }

    char signature[256] = "\0"; {
        cscat(signature, "(");
        for (int i = 0; i < sFinderArgCount; ++i) {
            ConcatenateSignature(signature, sFinderArgClasses[i]);
        }
        cscat(signature, ")");

        if (type == _NJNI_METHOD_INIT) {
            //NOTE: the signature of return type of "init" method is "V".
            cscat(signature, "V");
        } else {
            ConcatenateSignature(signature, sFinderRetClass);
        }
    }

    NJNIMethod key = {0};
    key.owningClass = cls;
    key.methodType  = type;
    key.methodName  = (char *)name;
    key.signature   = signature;

    NJNIMethod *method = NULL;
    nsyn() {
        //if the method already exists.
        method = blvalue(&list, pw(&key)).asptr;
        if (method) {
            break;
        }

        //find the method.
        JNIEnv *env = GetEnv();
        jmethodID methodID = NULL;
        if (type == _NJNI_METHOD_STATIC) {
            methodID = (*env)->GetStaticMethodID(env, cls->jniRef, name, signature);
        } else {
            methodID = (*env)->GetMethodID(env, cls->jniRef, name, signature);
        }
        if (!methodID) {
            break;
        }

        //create the method object.
        method = JNIMethodCreate();
        method->owningClass = NRetain(cls);
        method->methodType  = type;
        method->methodName  = csdup(name);
        method->signature   = csdup(signature);
        method->jniMethodID = methodID;

        method->retClass = NRetain(sFinderRetClass);
        method->argCount = sFinderArgCount;
        for (int i = 0; i < sFinderArgCount; ++i) {
            method->argClasses[i] = NRetain(sFinderArgClasses[i]);
        }

        blinsert(&list, pw(method), pw(method));
    }

    //NOTE: clear arguments.
    NJNIFinderReset();

    return method;
}

NJNIMethod *NJNIFindInitMethod(NJNIClass *cls) {
    if (!cls) {
        return NULL;
    }

    if (!sFinderRetClass) {
        //"init" method returns an instance of the class itself.
        NJNIFinderSetRet(cls);
    }
    return FindMethod(cls, _NJNI_METHOD_INIT, "<init>");
}

NJNIMethod *NJNIFindInstanceMethod(NJNIClass *cls, const char *name) {
    if (!cls || !name || *name == '\0') {
        return NULL;
    }

    if (!sFinderRetClass) {
        NJNIClass *retClass = ImportClass(_NJNI_CLASS_VOID, NULL, NULL);
        NJNIFinderSetRet(retClass);
    }
    return FindMethod(cls, _NJNI_METHOD_INSTANCE, name);
}

NJNIMethod *NJNIFindStaticMethod(NJNIClass *cls, const char *name) {
    if (!cls || !name || *name == '\0') {
        return NULL;
    }

    if (!sFinderRetClass) {
        NJNIClass *retClass = ImportClass(_NJNI_CLASS_VOID, NULL, NULL);
        NJNIFinderSetRet(retClass);
    }
    return FindMethod(cls, _NJNI_METHOD_STATIC, name);
}

static nthreadlocal int    sCallerArgCount = 0;
static nthreadlocal char   sCallerArgSigns[_NJNI_MAX_ARG_NUM] = {0};
static nthreadlocal jvalue sCallerArgWords[_NJNI_MAX_ARG_NUM] = {0};

void NJNICallerReset(void) {
    if (sCallerArgCount == 0) {
        return;
    }

    JNIEnv *env = GetEnv();
    for (int i = 0; i < sCallerArgCount; ++i) {
        if (sCallerArgSigns[i] == 'l' && sCallerArgWords[i].l) {
            (*env)->DeleteGlobalRef(env, sCallerArgWords[i].l);
        }
    }
    sCallerArgCount = 0;
}

static bool CallerCanPush(void) {
    return sCallerArgCount < _NJNI_MAX_ARG_NUM;
}

static void CallerPush(char type, jvalue word) {
    sCallerArgSigns[sCallerArgCount] = type;
    sCallerArgWords[sCallerArgCount] = word;
    sCallerArgCount += 1;
}

void NJNICallerPushBoolean(bool arg) {
    if (CallerCanPush()) {
        jvalue word = {0};
        word.z = arg;
        CallerPush('z', word);
    }
}

void NJNICallerPushLong(int64_t arg) {
    if (CallerCanPush()) {
        jvalue word = {0};
        word.j = arg;
        CallerPush('j', word);
    }
}

void NJNICallerPushDouble(double arg) {
    if (CallerCanPush()) {
        jvalue word = {0};
        word.d = arg;
        CallerPush('d', word);
    }
}

void NJNICallerPushString(NString *arg) {
    if (!CallerCanPush()) {
        return;
    }

    jstring globalRef = NULL; {
        JNIEnv *env = GetEnv();
        jstring localRef = LocalJNIString(env, arg);
        if (localRef) {
            globalRef = (*env)->NewGlobalRef(env, localRef);
            (*env)->DeleteLocalRef(env, localRef);
        }
    }

    jvalue word = {0};
    word.l = globalRef;
    CallerPush('l', word);
}

void NJNICallerPushObject(NJNIObject *arg) {
    if (!CallerCanPush()) {
        return;
    }

    jobject globalRef = NULL;
    if (arg && arg->jniRef) {
        JNIEnv *env = GetEnv();
        globalRef = (*env)->NewGlobalRef(env, arg->jniRef);
    }

    jvalue word = {0};
    word.l = globalRef;
    CallerPush('l', word);
}

static jvalue CastBooleanTo(_NJNIClassType type, jboolean src) {
    jvalue dst = {0};
    switch (type) {
        case _NJNI_CLASS_VOID   : break;
        case _NJNI_CLASS_BOOLEAN: dst.z = src ? 1 : 0; break;
        case _NJNI_CLASS_CHAR   : break;
        case _NJNI_CLASS_BYTE   : dst.b = src ? 1 : 0; break;
        case _NJNI_CLASS_SHORT  : dst.s = src ? 1 : 0; break;
        case _NJNI_CLASS_INT    : dst.i = src ? 1 : 0; break;
        case _NJNI_CLASS_LONG   : dst.j = src ? 1 : 0; break;
        case _NJNI_CLASS_FLOAT  : dst.f = src ? 1 : 0; break;
        case _NJNI_CLASS_DOUBLE : dst.d = src ? 1 : 0; break;
        default/*STR, OBJ, ARR*/: ;
    }
    return dst;
}

static jvalue CastLongTo(_NJNIClassType type, jlong src) {
    jvalue dst = {0};
    switch (type) {
        case _NJNI_CLASS_VOID   : break;
        case _NJNI_CLASS_BOOLEAN: dst.z = src ? 1 : 0; break;
        case _NJNI_CLASS_CHAR   : dst.c = (jchar )src; break;
        case _NJNI_CLASS_BYTE   : dst.b = (jbyte )src; break;
        case _NJNI_CLASS_SHORT  : dst.s = (jshort)src; break;
        case _NJNI_CLASS_INT    : dst.i = (jint  )src; break;
        case _NJNI_CLASS_LONG   : dst.j = /*long*/src; break;
        case _NJNI_CLASS_FLOAT  : dst.f = /*flt */src; break;
        case _NJNI_CLASS_DOUBLE : dst.d = /*dbl */src; break;
        default/*STR, OBJ, ARR*/: ;
    }
    return dst;
}

static jvalue CastDoubleTo(_NJNIClassType type, jdouble src) {
    jvalue dst = {0};
    switch (type) {
        case _NJNI_CLASS_VOID   : break;
        case _NJNI_CLASS_BOOLEAN: dst.z = src ? 1 : 0; break;
        case _NJNI_CLASS_CHAR   : break;
        case _NJNI_CLASS_BYTE   : dst.b = (jbyte )src; break;
        case _NJNI_CLASS_SHORT  : dst.s = (jshort)src; break;
        case _NJNI_CLASS_INT    : dst.i = (jint  )src; break;
        case _NJNI_CLASS_LONG   : dst.j = (jlong )src; break;
        case _NJNI_CLASS_FLOAT  : dst.f = (jfloat)src; break;
        case _NJNI_CLASS_DOUBLE : dst.d = /*dbl */src; break;
        default/*STR, OBJ, ARR*/: ;
    }
    return dst;
}

static jvalue CastObjectTo(_NJNIClassType type, jobject src) {
    jvalue dst = {0};
    if (type == _NJNI_CLASS_STRING
     || type == _NJNI_CLASS_OBJECT
     || type == _NJNI_CLASS_ARRAY)
    {
        dst.l = src;
    }
    return dst;
}

static jvalue CallMethod(JNIEnv *env, NJNIObject *object, NJNIMethod *method, bool *error) {
    jvalue values[_NJNI_MAX_ARG_NUM] = {0};

    if (!method) {
        *error = true;
        return *values;
    }
    if (method->methodType == _NJNI_METHOD_INSTANCE && !object) {
        *error = true;
        return *values;
    }
    if (sCallerArgCount < method->argCount) {
        _NError("only %d arguments passed, but %d required", sCallerArgCount, method->argCount);
        *error = true;
        return *values;
    }

    for (int i = 0; i < method->argCount; ++i) {
        _NJNIClassType type = method->argClasses[i]->classType;
        switch (sCallerArgSigns[i]) {
            case 'z': values[i] = CastBooleanTo(type, sCallerArgWords[i].z); break;
            case 'j': values[i] = CastLongTo   (type, sCallerArgWords[i].j); break;
            case 'd': values[i] = CastDoubleTo (type, sCallerArgWords[i].d); break;
            case 'l': values[i] = CastObjectTo (type, sCallerArgWords[i].l); break;
            default : ;
        }
    }

    jmethodID mid = method->jniMethodID;
    jvalue ret = {0};
    if (method->methodType == _NJNI_METHOD_INIT) {
        jclass cls = method->owningClass->jniRef;
        ret.l = (*env)->NewObjectA(env, cls, mid, values);

    } else if (method->methodType == _NJNI_METHOD_INSTANCE) {
        jobject obj = object->jniRef;
        switch (method->retClass->classType) {
            case _NJNI_CLASS_VOID   : /*...*/ (*env)->CallVoidMethodA   (env, obj, mid, values); break;
            case _NJNI_CLASS_BOOLEAN: ret.z = (*env)->CallBooleanMethodA(env, obj, mid, values); break;
            case _NJNI_CLASS_CHAR   : ret.c = (*env)->CallCharMethodA   (env, obj, mid, values); break;
            case _NJNI_CLASS_BYTE   : ret.b = (*env)->CallByteMethodA   (env, obj, mid, values); break;
            case _NJNI_CLASS_SHORT  : ret.s = (*env)->CallShortMethodA  (env, obj, mid, values); break;
            case _NJNI_CLASS_INT    : ret.i = (*env)->CallIntMethodA    (env, obj, mid, values); break;
            case _NJNI_CLASS_LONG   : ret.j = (*env)->CallLongMethodA   (env, obj, mid, values); break;
            case _NJNI_CLASS_FLOAT  : ret.f = (*env)->CallFloatMethodA  (env, obj, mid, values); break;
            case _NJNI_CLASS_DOUBLE : ret.d = (*env)->CallDoubleMethodA (env, obj, mid, values); break;
            default/*STR, OBJ, ARR*/: ret.l = (*env)->CallObjectMethodA (env, obj, mid, values);
        }

    } else /* methodType == _NJNI_METHOD_STATIC */ {
        jclass cls = method->owningClass->jniRef;
        switch (method->retClass->classType) {
            case _NJNI_CLASS_VOID   : /*...*/ (*env)->CallStaticVoidMethodA   (env, cls, mid, values); break;
            case _NJNI_CLASS_BOOLEAN: ret.z = (*env)->CallStaticBooleanMethodA(env, cls, mid, values); break;
            case _NJNI_CLASS_CHAR   : ret.c = (*env)->CallStaticCharMethodA   (env, cls, mid, values); break;
            case _NJNI_CLASS_BYTE   : ret.b = (*env)->CallStaticByteMethodA   (env, cls, mid, values); break;
            case _NJNI_CLASS_SHORT  : ret.s = (*env)->CallStaticShortMethodA  (env, cls, mid, values); break;
            case _NJNI_CLASS_INT    : ret.i = (*env)->CallStaticIntMethodA    (env, cls, mid, values); break;
            case _NJNI_CLASS_LONG   : ret.j = (*env)->CallStaticLongMethodA   (env, cls, mid, values); break;
            case _NJNI_CLASS_FLOAT  : ret.f = (*env)->CallStaticFloatMethodA  (env, cls, mid, values); break;
            case _NJNI_CLASS_DOUBLE : ret.d = (*env)->CallStaticDoubleMethodA (env, cls, mid, values); break;
            default/*STR, OBJ, ARR*/: ret.l = (*env)->CallStaticObjectMethodA (env, cls, mid, values);
        }
    }

    //NOTE: clear arguments.
    NJNICallerReset();

    return ret;
}

void NJNICallVoid(NJNIObject *object, NJNIMethod *method) {
    JNIEnv *env = GetEnv();
    bool    err = false;
    jvalue  ret = CallMethod(env, object, method, &err);

    if (err) {
        return;
    }

    _NJNIClassType retType = method->retClass->classType;
    if (retType == _NJNI_CLASS_STRING
     || retType == _NJNI_CLASS_OBJECT
     || retType == _NJNI_CLASS_ARRAY)
    {
        (*env)->DeleteLocalRef(env, ret.l);
    }
}

bool NJNICallBoolean(NJNIObject *object, NJNIMethod *method) {
    JNIEnv *env = GetEnv();
    bool    err = false;
    jvalue  ret = CallMethod(env, object, method, &err);

    if (err) {
        return false;
    }

    _NJNIClassType retType = method->retClass->classType;
    if (retType == _NJNI_CLASS_STRING
     || retType == _NJNI_CLASS_OBJECT
     || retType == _NJNI_CLASS_ARRAY)
    {
        (*env)->DeleteLocalRef(env, ret.l);
    }
    return (bool)ret.j;
}

int64_t NJNICallLong(NJNIObject *object, NJNIMethod *method) {
    JNIEnv *env = GetEnv();
    bool    err = false;
    jvalue  ret = CallMethod(env, object, method, &err);

    if (err) {
        return 0;
    }

    switch (method->retClass->classType) {
        case _NJNI_CLASS_VOID   : return 0;
        case _NJNI_CLASS_BOOLEAN: return ret.z ? 1 : 0;
        case _NJNI_CLASS_CHAR   : return ret.c;
        case _NJNI_CLASS_BYTE   : return ret.b;
        case _NJNI_CLASS_SHORT  : return ret.s;
        case _NJNI_CLASS_INT    : return ret.i;
        case _NJNI_CLASS_LONG   : return ret.j;
        case _NJNI_CLASS_FLOAT  : return (int64_t)ret.f;
        case _NJNI_CLASS_DOUBLE : return (int64_t)ret.d;

        default/*STR, OBJ, ARR*/: {
            (*env)->DeleteLocalRef(env, ret.l);
            return 0;
        }
    }
}

double NJNICallDouble(NJNIObject *object, NJNIMethod *method) {
    JNIEnv *env = GetEnv();
    bool    err = false;
    jvalue  ret = CallMethod(env, object, method, &err);

    if (err) {
        return 0;
    }

    switch (method->retClass->classType) {
        case _NJNI_CLASS_VOID   : return 0.0;
        case _NJNI_CLASS_BOOLEAN: return ret.z ? 1.0 : 0.0;
        case _NJNI_CLASS_CHAR   : return ret.c;
        case _NJNI_CLASS_BYTE   : return ret.b;
        case _NJNI_CLASS_SHORT  : return ret.s;
        case _NJNI_CLASS_INT    : return ret.i;
        case _NJNI_CLASS_LONG   : return ret.j;
        case _NJNI_CLASS_FLOAT  : return ret.f;
        case _NJNI_CLASS_DOUBLE : return ret.d;

        default/*STR, OBJ, ARR*/: {
            (*env)->DeleteLocalRef(env, ret.l);
            return 0.0;
        }
    }
}

NString *NJNICallRetainString(NJNIObject *object, NJNIMethod *method) {
    JNIEnv *env = GetEnv();
    bool    err = false;
    jvalue  ret = CallMethod(env, object, method, &err);

    if (err) {
        return NULL;
    }

    _NJNIClassType retType = method->retClass->classType;
    if (retType == _NJNI_CLASS_STRING) {
        NString *string = CreateStringFromJNI(env, ret.l);
        (*env)->DeleteLocalRef(env, ret.l);
        return string;
    } if (retType == _NJNI_CLASS_OBJECT || retType == _NJNI_CLASS_ARRAY) {
        (*env)->DeleteLocalRef(env, ret.l);
        return NULL;
    } else {
        return NULL;
    }
}

NJNIObject *NJNICallRetainObject(NJNIObject *object, NJNIMethod *method) {
    JNIEnv *env = GetEnv();
    bool    err = false;
    jvalue  ret = CallMethod(env, object, method, &err);

    if (err) {
        return NULL;
    }

    _NJNIClassType retType = method->retClass->classType;
    if (retType == _NJNI_CLASS_STRING
     || retType == _NJNI_CLASS_OBJECT
     || retType == _NJNI_CLASS_ARRAY)
    {
        jobject globalRef = (*env)->NewGlobalRef(env, ret.l);
        (*env)->DeleteLocalRef(env, ret.l);

        NJNIObject *retObject = JNIObjectCreate();
        retObject->cls = NRetain(method->retClass);
        retObject->jniRef = globalRef;
        return retObject;
    }
    return NULL;
}
