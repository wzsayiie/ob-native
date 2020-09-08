#pragma once

#include "xcursor.h"

defstruct(xlist);

xlist *xlistnew(int    rnum, size_t size, fdel del);
void   xlistdel(xlist *list);
void __xlistdel(void **addr);

void xlistadd (xlist *list, int    idx , void **item);
void xlistpush(xlist *list, void **item);

void xlistrm (xlist *list, int idx);
void xlistpop(xlist *list);

int      xlistlen  (xlist *list);
icursor *xlistrange(xlist *list);
void   **xlistitem (xlist *list, int idx);
void   **xlistlast (xlist *list);

#define genlist(lname, itype, delfn)                                  \
/**/                                                                  \
/**/    defstruct(lname);                                             \
/**/                                                                  \
/**/    inlinable lname *lname##new(void) {                           \
/**/        return (lname *)xlistnew(0, sizeof(itype), delfn);        \
/**/    }                                                             \
/**/    inlinable void lname##del(lname *list) {                      \
/**/        xlistdel((xlist *)list);                                  \
/**/    }                                                             \
/**/    inlinable void lname##add(lname *list, int idx, itype item) { \
/**/        xlistadd((xlist *)list, idx, (void **)&item);             \
/**/    }                                                             \
/**/    inlinable void lname##push(lname *list, itype item) {         \
/**/        xlistpush((xlist *)list, (void **)&item);                 \
/**/    }                                                             \
/**/    inlinable void lname##rm(lname *list, int idx) {              \
/**/        xlistrm((xlist *)list, idx);                              \
/**/    }                                                             \
/**/    inlinable void lname##pop(lname *list) {                      \
/**/        xlistpop((xlist *)list);                                  \
/**/    }                                                             \
/**/    inlinable int lname##len(lname *list) {                       \
/**/        return xlistlen((xlist *)list);                           \
/**/    }                                                             \
/**/    inlinable icursor *lname##range(lname *list) {                \
/**/        return xlistrange((xlist *)list);                         \
/**/    }                                                             \
/**/    inlinable itype lname##item(lname *list, int idx) {           \
/**/        void **addr = xlistitem((xlist *)list, idx);              \
/**/        return addr ? *(itype *)addr : (itype)0;                  \
/**/    }                                                             \
/**/    inlinable itype lname##last(lname *list) {                    \
/**/        void **addr = xlistlast((xlist *)list);                   \
/**/        return addr ? *(itype *)addr : (itype)0;                  \
/**/    }

genlist(ilist, int   , NULL)
genlist(flist, float , NULL)
genlist(slist, ustr *, __ustrdel)
