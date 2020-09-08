#pragma once

#include "xcursor.h"

defstruct(xmap);

xmap  *xmapnew(int    rnum, size_t size, fdel del);
void   xmapdel(xmap  *map );
void __xmapdel(void **addr);

void xmapsset(xmap *map, ustr *key, void **val);
void xmapiset(xmap *map, int   key, void **val);

void xmapsrm(xmap *map, ustr *key);
void xmapirm(xmap *map, int   key);

int      xmaplen   (xmap *map);
scursor *xmapsrange(xmap *map);
icursor *xmapirange(xmap *map);
void   **xmapsget  (xmap *map, ustr *key);
void   **xmapiget  (xmap *map, int   key);

#define genmap(mname, vtype, delfn)                                    \
/**/                                                                   \
/**/    defstruct(mname);                                              \
/**/                                                                   \
/**/    inlinable mname *mname##new(int rnum, size_t size, fdel del) { \
/**/        return (mname *)xmapnew(rnum, size, del);                  \
/**/    }                                                              \
/**/    inlinable void mname##del(mname *map) {                        \
/**/        xmapdel((xmap *)map);                                      \
/**/    }                                                              \
/**/    inlinable void mname##sset(mname *map, ustr *key, vtype val) { \
/**/        xmapsset((xmap *)map, key, (void **)&val);                 \
/**/    }                                                              \
/**/    inlinable void mname##iset(mname *map, int key, vtype val) {   \
/**/        xmapiset((xmap *)map, key, (void **)&val);                 \
/**/    }                                                              \
/**/    inlinable void mname##srm(mname *map, ustr *key) {             \
/**/        xmapsrm((xmap *)map, key);                                 \
/**/    }                                                              \
/**/    inlinable void mname##irm(mname *map, int key) {               \
/**/        xmapirm((xmap *)map, key);                                 \
/**/    }                                                              \
/**/    inlinable int mname##len(mname *map) {                         \
/**/        return xmaplen((xmap *)map);                               \
/**/    }                                                              \
/**/    inlinable scursor *mname##srange(mname *map) {                 \
/**/        return xmapsrange((xmap *)map);                            \
/**/    }                                                              \
/**/    inlinable icursor *mname##irange(mname *map) {                 \
/**/        return xmapirange((xmap *)map);                            \
/**/    }                                                              \
/**/    inlinable vtype mname##sget(mname *map, ustr *key) {           \
/**/        void **addr = xmapsget((xmap *)map, key);                  \
/**/        return addr ? *(vtype *)addr : (vtype)0;                   \
/**/    }                                                              \
/**/    inlinable vtype mname##iget(mname *map, int key) {             \
/**/        void **addr = xmapiget((xmap *)map, key);                  \
/**/        return addr ? *(vtype *)addr : (vtype)0;                   \
/**/    }

genmap(imap, int   , NULL)
genmap(fmap, float , NULL)
genmap(smap, ustr *, __ustrdel)
