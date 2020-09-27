#pragma once

#include "cont.h"

defstruct(smap);

smap *newsmap(void);
void _delsmap(smap *map);

void smap_seti(smap *map, cstr *key, int   val);
void smap_setf(smap *map, cstr *key, float val);
void smap_set (smap *map, cstr *key, cref  val);

int   smap_geti(smap *map, cstr *key);
float smap_getf(smap *map, cstr *key);
cref  smap_get (smap *map, cstr *key);

int  smap_len(smap *map);
bool smap_has(smap *map, cstr *key);
void smap_rm (smap *map, cstr *key);

cranger *smap_pairs(smap *map);
