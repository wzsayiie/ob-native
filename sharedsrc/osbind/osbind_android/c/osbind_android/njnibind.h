#pragma once

// #include "basis.h"

// nenum(NJTypeKind) {
//     NJBool   =  1,
//     NJChar   =  2,
//     NJByte   =  3,
//     NJShort  =  4,
//     NJInt    =  5,
//     NJLong   =  6,
//     NJFloat  =  7,
//     NJDouble =  8,
//     NJString =  9,
//     NJClass  = 10,

//     NJBoolArray   = 11,
//     NJCharArray   = 12,
//     NJByteArray   = 13,
//     NJShortArray  = 14,
//     NJIntArray    = 15,
//     NJLongArray   = 16,
//     NJFloatArray  = 17,
//     NJDoubleArray = 18,
//     NJStringArray = 19,
//     NJClassArray  = 20,
// };

// nenum(NJMethodKind) {
//     NJConstructMethod = 1,
//     NJStaticMethod    = 2,
//     NJInstanceMethod  = 3,
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
