#pragma once

#include "macro.h"

defenum(uencode) {
    UTF8  = 1,
    UTF16 = 2,
    UTF32 = 3,
};

typedef uint16_t char16_t;
typedef uint32_t char32_t;

defstruct(ustr);

ustr *ustrnew  (const char     *raw);
ustr *ustrnew16(const char16_t *raw);
ustr *ustrnew32(const char32_t *raw);

//NOTE:
//"ustrnewbt" will generate a object that takes over "bytes".
//and "ustrcpybt" will create a copy of "bytes".
ustr *ustrnewbt(const void *bytes, size_t size, uencode enc);
ustr *ustrcpybt(const void *bytes, size_t size, uencode enc);

void   ustrdel(ustr  *str );
void __ustrdel(void **addr);

//NOTE:
//ustrlen() returns the number of characters in the string,
//not the byte length of the string.
int ustrlen(ustr *str);

char32_t ustritem(ustr *str, int idx);

//get the raw string stored inside "ustr",
//and use "ustrbsize" to get the byte length of the string.
const char     *ustrraw  (ustr *str);
const char16_t *ustrraw16(ustr *str);
const char32_t *ustrraw32(ustr *str);

int ustrbsize  (ustr *str);
int ustrbsize16(ustr *str);
int ustrbsize32(ustr *str);

//convert a string literal to "ustr", and store it on current thread.
//NOTE: the returned value don't need to be deleted.
ustr *U8 (const char     *raw);
ustr *U16(const char16_t *raw);
ustr *U32(const char32_t *raw);

ustr *ustrcpy    (ustr *str);
void  ustrset    (ustr *str, ustr *val);
int   ustrcmp    (ustr *str, ustr *val);
int   ustrcasecmp(ustr *str, ustr *val);
void  ustrcat    (ustr *str, ustr *add);
