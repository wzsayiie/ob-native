#pragma once

#include "basis.h"
#include <jni.h>

/*
//import a jni type:

nenum(_NJNIClassType, {
    _NJNI_CLASS_VOID    =  1,
    _NJNI_CLASS_BOOLEAN =  2,
    _NJNI_CLASS_CHAR    =  3,
    _NJNI_CLASS_BYTE    =  4,
    _NJNI_CLASS_SHORT   =  5,
    _NJNI_CLASS_INT     =  6,
    _NJNI_CLASS_LONG    =  7,
    _NJNI_CLASS_FLOAT   =  8,
    _NJNI_CLASS_DOUBLE  =  9,
    _NJNI_CLASS_STRING  = 10,
    _NJNI_CLASS_OBJECT  = 11,
    _NJNI_CLASS_ARRAY   = 12,
});

nclass(NJNIClass, NObject, {
    union {
        NObject Object;
        NObject Super ;
    };

    _NJNIClassType classType;
    char          *className;
    jclass         jniRef   ;
    NJNIClass     *itemClass;
});

nfunc(NJNIClass *, NJNIImportClass, (const char *name));
nfunc(NJNIClass *, NJNIImportArrayClass, (NJNIClass *itemClass));

//create a jni array:

nclass(NJNIObject, NObject, {
    union {
        NObject Object;
        NObject Super ;
    };

    NJNIClass *cls   ;
    jobject   *jniRef;
});

nfunc(NJNIObject *, NJNICreateArray, (NJNIClass *cls, int count));

nfunc(int, NJNIArrayCount, (NJNIObject *array));

nfunc(void, NJNISetBooleanItem, (NJNIObject *array, int index, bool        item));
nfunc(void, NJNISetLongItem   , (NJNIObject *array, int index, int64_t     item));
nfunc(void, NJNISetDoubleItem , (NJNIObject *array, int index, double      item));
nfunc(void, NJNISetStringItem , (NJNIObject *array, int index, NString    *item));
nfunc(void, NJNISetObjectItem , (NJNIObject *array, int index, NJNIObject *item));

nfunc(bool        , NJNIBooleanItem     , (NJNIObject *array, int index));
nfunc(int64_t     , NJNILongItem        , (NJNIObject *array, int index));
nfunc(double      , NJNIDoubleItem      , (NJNIObject *array, int index));
nfunc(NString    *, NJNIRetainStringItem, (NJNIObject *array, int index));
nfunc(NJNIObject *, NJNIRetainObjectItem, (NJNIObject *array, int index));
nfunc(NData      *, NJNICopyPODItems    , (NJNIObject *array));

//find a jni method:

nenum(_NJNIMethodType, {
    _NJNI_METHOD_INIT     = 1,
    _NJNI_METHOD_INSTANCE = 2,
    _NJNI_METHOD_STATIC   = 3,
});

#define _NJNI_MAX_ARG_NUM 4

nclass(NJNIMethod, NObject, {
    union {
        NObject Object;
        NObject Super ;
    };

    NJNIClass      *owningClass;
    _NJNIMethodType methodType ;
    char           *methodName ;
    char           *signature  ;
    jmethodID       jniMethodID;

    NJNIClass *retClass;
    int        argCount;
    NJNIClass *argClasses[_NJNI_MAX_ARG_NUM];
});

nfunc(void, NJNIFinderPrepare, (void));
nfunc(void, NJNIFinderSetRet , (NJNIClass *ret));
nfunc(void, NJNIFinderAddArg , (NJNIClass *arg));

nfunc(NJNIMethod *, NJNIFindInitMethod    , (NJNIClass *cls));
nfunc(NJNIMethod *, NJNIFindInstanceMethod, (NJNIClass *cls, const char *name));
nfunc(NJNIMethod *, NJNIFindStaticMethod  , (NJNIClass *cls, const char *name));

//call a jni method:

nfunc(void, NJNICallerPrepareCall, (void));
nfunc(void, NJNICallerPushBoolean, (bool        arg));
nfunc(void, NJNICallerPushLong   , (int64_t     arg));
nfunc(void, NJNICallerPushDouble , (double      arg));
nfunc(void, NJNICallerPushString , (NString    *arg));
nfunc(void, NJNICallerPushObject , (NJNIObject *arg));

nfunc(void        , NJNICallVoid        , (NJNIObject *object, NJNIMethod *method));
nfunc(bool        , NJNICallBoolean     , (NJNIObject *object, NJNIMethod *method));
nfunc(int64_t     , NJNICallLong        , (NJNIObject *object, NJNIMethod *method));
nfunc(double      , NJNICallDouble      , (NJNIObject *object, NJNIMethod *method));
nfunc(NString    *, NJNICallRetainString, (NJNIObject *object, NJNIMethod *method));
nfunc(NJNIObject *, NJNICallRetainObject, (NJNIObject *object, NJNIMethod *method));
*/
