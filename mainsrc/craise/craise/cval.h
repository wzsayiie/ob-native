#pragma once

#include "clist.h"
#include "cstr.h"
#include "imap.h"
#include "smap.h"

defenum(ctype) {
    TYPE_BOOL,

    TYPE_INT  ,
    TYPE_INT8 ,
    TYPE_INT16,
    TYPE_INT32,
    TYPE_INT64,

    TYPE_FLT,
    TYPE_DBL,

    TYPE_PTR,

    TYPE_CSTR ,
    TYPE_CLIST,
    TYPE_IMAP ,
    TYPE_SMAP ,
};

defstruct(cval) {
    ctype type;
    union {
        int64_t i64v;
        double  dblv;
        void   *ptrv;
    };
};

cval mkbool (bool    val);
cval mkint  (int     val);
cval mkint8 (int8_t  val);
cval mkint16(int16_t val);
cval mkint32(int32_t val);
cval mkint64(int64_t val);
cval mkflt  (float   val);
cval mkdbl  (double  val);
cval mkptr  (void   *val);
cval mkcstr (cstr   *val);
cval mkclist(clist  *val);
cval mkimap (imap   *val);
cval mksmap (smap   *val);

bool isnum  (cval val); //is inum or fnum.
bool isinum (cval val); //is int, int8, int16, int32 or int64.
bool isfnum (cval val); //is flt or dbl.
bool isbool (cval val);
bool isint  (cval val);
bool isint8 (cval val);
bool isint16(cval val);
bool isint32(cval val);
bool isint64(cval val);
bool isflt  (cval val);
bool isdbl  (cval val);
bool isptr  (cval val);
bool iscstr (cval val);
bool isclist(cval val);
bool isimap (cval val);
bool issmap (cval val);

bool    asbool (cval val);
int     asint  (cval val);
int8_t  asint8 (cval val);
int16_t asint16(cval val);
int32_t asint32(cval val);
int64_t asint64(cval val);
float   asflt  (cval val);
double  asdbl  (cval val);
void   *asptr  (cval val);
cstr   *ascstr (cval val);
clist  *asclist(cval val);
imap   *asimap (cval val);
smap   *assmap (cval val);
