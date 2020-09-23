#pragma once

#include "cobj.h"

defenum(sencode) {
    SENC_UTF8 ,
    SENC_UTF16,
    SENC_UTF32,
};

typedef int16_t char16_t;
typedef int32_t char32_t;

defstruct(cstr);

cstr *cstrnewbt(const void *bytes, size_t size, sencode enc);

cstr *cstrnew  (const char     *raw);
cstr *cstrnew16(const char16_t *raw);
cstr *cstrnew32(const char32_t *raw);

//for bytes process:
const void *cstrbytes(cstr *str, sencode enc);
int cstrbsize(cstr *str, sencode enc);

//for characters process:
char32_t cstrchar(cstr *str, sencode enc, int idx);
int cstrclen(cstr *str, sencode env);

int  cstrlen(cstr *str);
int  cstrcmp(cstr *str, cstr *tht);
void cstrcat(cstr *str, cstr *tht);
