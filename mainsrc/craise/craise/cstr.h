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

cstr *newcstrbt(const void     *bytes, size_t size, sencode enc);
cstr *newcstr  (const char     *raw);
cstr *newcstr16(const char16_t *raw);
cstr *newcstr32(const char32_t *raw);
void _delcstr  (cstr *str);

//for bytes process:
const void *cstr_bytes(cstr *str, sencode enc);
int cstr_bsize(cstr *str, sencode enc);

//for characters process:
char32_t cstr_char(cstr *str, sencode enc, int idx);
int cstr_clen(cstr *str, sencode env);

int  cstr_len(cstr *str);
int  cstr_cmp(cstr *str, cstr *tht);
void cstr_cat(cstr *str, cstr *tht);
