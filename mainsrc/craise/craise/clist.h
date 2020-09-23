#pragma once

#include "ccont.h"

defstruct(clist);

clist *clistnew(void);

//for list process:
void  clistaddi(clist *list, int idx, int   val);
void  clistaddf(clist *list, int idx, float val);
void  clistadd (clist *list, int idx, cref  val);
int   clistgeti(clist *list, int idx);
float clistgetf(clist *list, int idx);
cref  clistget (clist *list, int idx);
void  clistrm  (clist *list, int idx);

//for stack process:
void  clistpushi(clist *list, int   val);
void  clistpushf(clist *list, float val);
void  clistpush (clist *list, cref  val);
int   clistlasti(clist *list);
float clistlastf(clist *list);
cref  clistlast (clist *list);
void  clistpop  (clist *list);

int clistlen(clist *list);

cranger *clistitems(clist *list);
