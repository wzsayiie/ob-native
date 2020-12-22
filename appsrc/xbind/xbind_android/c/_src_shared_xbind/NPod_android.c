#include <jni.h>
#include "basis.h"

JNIEXPORT jboolean JNICALL Java_src_shared_xbind_NPod_booleanValue(JNIEnv *e, jclass c, jlong w) {return NBoolWord  (w);}
JNIEXPORT jbyte    JNICALL Java_src_shared_xbind_NPod_sbyteValue  (JNIEnv *e, jclass c, jlong w) {return NInt8Word  (w);}
JNIEXPORT jshort   JNICALL Java_src_shared_xbind_NPod_shortValue  (JNIEnv *e, jclass c, jlong w) {return NInt16Word (w);}
JNIEXPORT jint     JNICALL Java_src_shared_xbind_NPod_intValue    (JNIEnv *e, jclass c, jlong w) {return NInt32Word (w);}
JNIEXPORT jlong    JNICALL Java_src_shared_xbind_NPod_longValue   (JNIEnv *e, jclass c, jlong w) {return NInt64Word (w);}
JNIEXPORT jbyte    JNICALL Java_src_shared_xbind_NPod_byteValue   (JNIEnv *e, jclass c, jlong w) {return NUInt8Word (w);}
JNIEXPORT jshort   JNICALL Java_src_shared_xbind_NPod_ushortValue (JNIEnv *e, jclass c, jlong w) {return NUInt16Word(w);}
JNIEXPORT jint     JNICALL Java_src_shared_xbind_NPod_uintValue   (JNIEnv *e, jclass c, jlong w) {return NUInt32Word(w);}
JNIEXPORT jlong    JNICALL Java_src_shared_xbind_NPod_ulongValue  (JNIEnv *e, jclass c, jlong w) {return NUInt64Word(w);}
JNIEXPORT jfloat   JNICALL Java_src_shared_xbind_NPod_floatValue  (JNIEnv *e, jclass c, jlong w) {return NFloatWord (w);}
JNIEXPORT jdouble  JNICALL Java_src_shared_xbind_NPod_doubleValue (JNIEnv *e, jclass c, jlong w) {return NDoubleWord(w);}
JNIEXPORT jlong    JNICALL Java_src_shared_xbind_NPod_ptrValue    (JNIEnv *e, jclass c, jlong w) {return w;}

JNIEXPORT jlong JNICALL Java_src_shared_xbind_NPod_booleanWord(JNIEnv *e, jclass c, jboolean v) {return NMakeBoolWord  (v);}
JNIEXPORT jlong JNICALL Java_src_shared_xbind_NPod_byteWord   (JNIEnv *e, jclass c, jbyte    v) {return NMakeInt8Word  (v);}
JNIEXPORT jlong JNICALL Java_src_shared_xbind_NPod_shortWord  (JNIEnv *e, jclass c, jshort   v) {return NMakeInt16Word (v);}
JNIEXPORT jlong JNICALL Java_src_shared_xbind_NPod_intWord    (JNIEnv *e, jclass c, jint     v) {return NMakeInt32Word (v);}
JNIEXPORT jlong JNICALL Java_src_shared_xbind_NPod_longWord   (JNIEnv *e, jclass c, jlong    v) {return NMakeInt64Word (v);}
JNIEXPORT jlong JNICALL Java_src_shared_xbind_NPod_floatWord  (JNIEnv *e, jclass c, jfloat   v) {return NMakeFloatWord (v);}
JNIEXPORT jlong JNICALL Java_src_shared_xbind_NPod_doubleWord (JNIEnv *e, jclass c, jdouble  v) {return NMakeDoubleWord(v);}
JNIEXPORT jlong JNICALL Java_src_shared_xbind_NPod_ptrWord    (JNIEnv *e, jclass c, jlong    v) {return v;}
