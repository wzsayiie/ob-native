#pragma once

// #include "basis.h"

// nenum(NJTypeKind) {
//     NJTYPE_BOOLEAN =  1,
//     NJTYPE_CHAR    =  2,
//     NJTYPE_BYTE    =  3,
//     NJTYPE_SHORT   =  4,
//     NJTYPE_INT     =  5,
//     NJTYPE_LONG    =  6,
//     NJTYPE_FLOAT   =  7,
//     NJTYPE_DOUBLE  =  8,
//     NJTYPE_STRING  =  9,
//     NJTYPE_CLASS   = 10,

//     NJTYPE_BOOLEAN_ARRAY = 11,
//     NJTYPE_CHAR_ARRAY    = 12,
//     NJTYPE_BYTE_ARRAY    = 13,
//     NJTYPE_SHORT_ARRAY   = 14,
//     NJTYPE_INT_ARRAY     = 15,
//     NJTYPE_LONG_ARRAY    = 16,
//     NJTYPE_FLOAT_ARRAY   = 17,
//     NJTYPE_DOUBLE_ARRAY  = 18,
//     NJTYPE_STRING_ARRAY  = 19,
//     NJTYPE_CLASS_ARRAY   = 20,

//     NJTYPE_VOID = 21,
// };

// nenum(NJMethodKind) {
//     NJMETHOD_CONSTRUCT = 1,
//     NJMETHOD_STATIC    = 2,
//     NJMETHOD_INSTANCE  = 3,
// };

// nclass(NJType  , NObject);
// nclass(NJMethod, NObject);
// nclass(NJObject, NObject);

// //import jni types:

// nfunc(NJType *, NJBasicType, (NJTypeKind  kind));
// nfunc(NJType *, NJClassType, (const char *path));

// //manipulate array types:

// nfunc(NJObject *, NJCreateArray, (NJTypeKind kind, int count));

// nfunc(int, NJArrayCount, (NJObject *array));

// nfunc(void, NJSetBoolItem  , (NJObject *array, int index, bool      item));
// nfunc(void, NJSetIntItem   , (NJObject *array, int index, int64_t   item));
// nfunc(void, NJSetFltItem   , (NJObject *array, int index, double    item));
// nfunc(void, NJSetStringItem, (NJObject *array, int index, NString  *item));
// nfunc(void, NJSetObjectItem, (NJObject *array, int index, NJObject *item));

// nfunc(bool   , NJBoolItem, (NJObject *array, int index));
// nfunc(int64_t, NJIntItem , (NJObject *array, int index));
// nfunc(double , NJFltItem , (NJObject *array, int index));

// nfunc(NString  *, NJRetainStringItem, (NJObject *array, int index));
// nfunc(NJObject *, NJRetainObjectItem, (NJObject *array, int index));

// //find jni methods:

// nfunc(void      , NJFinderSetArgs, (NType *arg0, NType *arg1, NType *arg2, NType *arg3));
// nfunc(void      , NJFinderSetRet , (NType *ret));
// nfunc(NJMethod *, NJFindMethod   , (NType *cls, NJMethodKind kind, const char *func));

// //invoke jni methods:

// nfunc(void, NJCallerPushBool  , (int reset, bool      arg));
// nfunc(void, NJCallerPushInt   , (int reset, int64_t   arg));
// nfunc(void, NJCallerPushFlt   , (int reset, double    arg));
// nfunc(void, NJCallerPushString, (int reset, NString  *arg));
// nfunc(void, NJCallerPushObject, (int reset, NJObject *arg));

// nfunc(void   , NJCallVoid, (NJObject *object, NJMethod *method));
// nfunc(bool   , NJCallBool, (NJObject *object, NJMethod *method));
// nfunc(int64_t, NJCallInt , (NJObject *object, NJMethod *method));
// nfunc(double , NJCallFlt , (NJObject *object, NJMethod *method));

// nfunc(NString  *, NJCallRetainString, (NJObject *object, NJMethod *method));
// nfunc(NJObject *, NJCallRetainObject, (NJObject *object, NJMethod *method));
