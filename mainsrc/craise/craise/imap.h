#pragma once

#include "ccont.h"

defstruct(imap);

imap *imapnew(void);

void imapseti(imap *map, int key, int   val);
void imapsetf(imap *map, int key, float val);
void imapset (imap *map, int key, cref  val);

int   imapgeti(imap *map, int key);
float imapgetf(imap *map, int key);
cref  imapget (imap *map, int key);

int  imaplen(imap *map);
bool imaphas(imap *map, int key);
void imaprm (imap *map, int key);

cranger *imappairs(imap *map);
