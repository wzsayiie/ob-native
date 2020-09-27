#pragma once

#include "cont.h"

defstruct(imap);

imap *newimap(void);
void _delimap(imap *map);

void imap_seti(imap *map, int key, int   val);
void imap_setf(imap *map, int key, float val);
void imap_set (imap *map, int key, cref  val);

int   imap_geti(imap *map, int key);
float imap_getf(imap *map, int key);
cref  imap_get (imap *map, int key);

int  imap_len(imap *map);
bool imap_has(imap *map, int key);
void imap_rm (imap *map, int key);

cranger *imap_pairs(imap *map);
