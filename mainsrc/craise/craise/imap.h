#pragma once

#include "cont.h"

defstruct(imap);

imap *newimap(void);
void _delimap(imap *map);

void imap_seti(imap *self, int key, int   val);
void imap_setf(imap *self, int key, float val);
void imap_set (imap *self, int key, cref  val);

int   imap_geti(imap *self, int key);
float imap_getf(imap *self, int key);
cref  imap_get (imap *self, int key);

void imap_clear (imap *self);
int  imap_len   (imap *self);
bool imap_exist (imap *self, int key);
void imap_remove(imap *self, int key);

cranger *imap_pairs(imap *self);
