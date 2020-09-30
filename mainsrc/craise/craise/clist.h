#pragma once

#include "cont.h"

defstruct(clist);

clist *newclist(void);
void  _delclist(clist *list);

//for list process:
void  clist_inserti(clist *self, int idx, int   val);
void  clist_insertf(clist *self, int idx, float val);
void  clist_insert (clist *self, int idx, cref  val);
int   clist_ati    (clist *self, int idx);
float clist_atf    (clist *self, int idx);
cref  clist_at     (clist *self, int idx);
void  clist_remove (clist *self, int idx);

//for stack process:
void  clist_pushi(clist *self, int   val);
void  clist_pushf(clist *self, float val);
void  clist_push (clist *self, cref  val);
int   clist_lasti(clist *self);
float clist_lastf(clist *self);
cref  clist_last (clist *self);
void  clist_pop  (clist *self);

void clist_clear(clist *self);
int  clist_len  (clist *self);

cranger *clist_items(clist *self);
