#pragma once

#include "ccont.h"

defstruct(smap);

smap *smapnew(void);

void smapseti(smap *map, cstr *key, int   val);
void smapsetf(smap *map, cstr *key, float val);
void smapset (smap *map, cstr *key, cref  val);

int   smapgeti(smap *map, cstr *key);
float smapgetf(smap *map, cstr *key);
cref  smapget (smap *map, cstr *key);

int  smaplen(smap *map);
bool smaphas(smap *map, cstr *key);
void smaprm (smap *map, cstr *key);

cranger *smappairs(smap *map);
