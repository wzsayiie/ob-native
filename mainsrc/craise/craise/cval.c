#include "cval.h"

cval mkbool (bool    v) {cval a; a.type = TYPE_BOOL ; a.i64v = v; return a;}
cval mkint  (int     v) {cval a; a.type = TYPE_INT  ; a.i64v = v; return a;}
cval mkint8 (int8_t  v) {cval a; a.type = TYPE_INT8 ; a.i64v = v; return a;}
cval mkint16(int16_t v) {cval a; a.type = TYPE_INT16; a.i64v = v; return a;}
cval mkint32(int32_t v) {cval a; a.type = TYPE_INT32; a.i64v = v; return a;}
cval mkint64(int64_t v) {cval a; a.type = TYPE_INT64; a.i64v = v; return a;}
cval mkflt  (float   v) {cval a; a.type = TYPE_FLT  ; a.dblv = v; return a;}
cval mkdbl  (double  v) {cval a; a.type = TYPE_DBL  ; a.dblv = v; return a;}
cval mkptr  (void   *v) {cval a; a.type = TYPE_PTR  ; a.ptrv = v; return a;}
cval mkcstr (cstr   *v) {cval a; a.type = TYPE_CSTR ; a.ptrv = v; return a;}
cval mkclist(clist  *v) {cval a; a.type = TYPE_CLIST; a.ptrv = v; return a;}
cval mkimap (imap   *v) {cval a; a.type = TYPE_IMAP ; a.ptrv = v; return a;}
cval mksmap (smap   *v) {cval a; a.type = TYPE_SMAP ; a.ptrv = v; return a;}

bool isnum(cval val) {
    return isinum(val) || isfnum(val);
}

bool isinum(cval val) {
    if (val.type == TYPE_INT  ) {return true;}
    if (val.type == TYPE_INT8 ) {return true;}
    if (val.type == TYPE_INT16) {return true;}
    if (val.type == TYPE_INT32) {return true;}
    if (val.type == TYPE_INT64) {return true;}

    return false;
}

bool isfnum(cval val) {
    if (val.type == TYPE_FLT) {return true;}
    if (val.type == TYPE_DBL) {return true;}

    return false;
}

bool isbool (cval v) {return v.type == TYPE_BOOL ;}
bool isint  (cval v) {return v.type == TYPE_INT  ;}
bool isint8 (cval v) {return v.type == TYPE_INT8 ;}
bool isint16(cval v) {return v.type == TYPE_INT16;}
bool isint32(cval v) {return v.type == TYPE_INT32;}
bool isint64(cval v) {return v.type == TYPE_INT64;}
bool isflt  (cval v) {return v.type == TYPE_FLT  ;}
bool isdbl  (cval v) {return v.type == TYPE_DBL  ;}
bool isptr  (cval v) {return v.type == TYPE_PTR  ;}
bool iscstr (cval v) {return v.type == TYPE_CSTR ;}
bool isclist(cval v) {return v.type == TYPE_CLIST;}
bool isimap (cval v) {return v.type == TYPE_IMAP ;}
bool issmap (cval v) {return v.type == TYPE_SMAP ;}

bool asbool(cval val) {
    if (isbool(val)) {return (bool)val.i64v;}
    if (isinum(val)) {return (bool)val.i64v;}
    if (isfnum(val)) {return (bool)val.dblv;}

    return val.ptrv != NULL;
}

int     asint  (cval val) {return (int    )asint64(val);}
int8_t  asint8 (cval val) {return (int8_t )asint64(val);}
int16_t asint16(cval val) {return (int16_t)asint64(val);}
int32_t asint32(cval val) {return (int32_t)asint64(val);}
float   asflt  (cval val) {return (float  )asdbl  (val);}

int64_t asint64(cval val) {
    if (isbool(val)) {return val.i64v;}
    if (isinum(val)) {return val.i64v;}
    
    if (isfnum(val)) {
        return (int64_t)val.dblv;
    }
    
    //a pointer can be casted to a int.
    return (int64_t)(intptr_t)val.ptrv;
}

double asdbl(cval val) {
    if (isbool(val)) {return (double)val.i64v;}
    if (isinum(val)) {return (double)val.i64v;}
    
    if (isfnum(val)) {
        return val.dblv;
    }
    
    return 0;
}

void  *asptr  (cval v) {return isptr  (v) ? v.ptrv : NULL;}
cstr  *ascstr (cval v) {return iscstr (v) ? v.ptrv : NULL;}
clist *asclist(cval v) {return isclist(v) ? v.ptrv : NULL;}
imap  *asimap (cval v) {return isimap (v) ? v.ptrv : NULL;}
smap  *assmap (cval v) {return issmap (v) ? v.ptrv : NULL;}
