#pragma once

#include "cont.h"

defstruct(smap);

smap *newsmap(void);
void _delsmap(smap *map);

void smap_seti(smap *self, cstr *key, int   val);
void smap_setf(smap *self, cstr *key, float val);
void smap_set (smap *self, cstr *key, cref  val);

int   smap_geti(smap *self, cstr *key);
float smap_getf(smap *self, cstr *key);
cref  smap_get (smap *self, cstr *key);

void smap_clear (smap *self);
int  smap_len   (smap *self);
bool smap_exist (smap *self, cstr *key);
void smap_remove(smap *self, cstr *key);

cranger *smap_pairs(smap *self);
