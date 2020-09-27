#pragma once

#include "cont.h"

defstruct(clist);

clist *newclist(void);
void  _delclist(clist *list);

//for list process:
void  clist_addi(clist *list, int idx, int   val);
void  clist_addf(clist *list, int idx, float val);
void  clist_add (clist *list, int idx, cref  val);
int   clist_ati (clist *list, int idx);
float clist_atf (clist *list, int idx);
cref  clist_at  (clist *list, int idx);
void  clist_rm  (clist *list, int idx);

//for stack process:
void  clist_pushi(clist *list, int   val);
void  clist_pushf(clist *list, float val);
void  clist_push (clist *list, cref  val);
int   clist_lasti(clist *list);
float clist_lastf(clist *list);
cref  clist_last (clist *list);
void  clist_pop  (clist *list);

int clist_len(clist *list);

cranger *clist_items(clist *list);
