#pragma once

#include "niterator.h"
#include "nmemory.h"
#include "nunicode.h"

nclass(NString, NObject, {
    union {
        NObject Object;
        NObject Super ;
    };

    int       length  ;
    char32_t *u32chars;
    char16_t *u16chars;
    char     *u8chars ;
});

void _NStringInitWithUTFBytes(NString *string, NUTFType type, const void *begin, const void *end);
void _NStringInitWithUTFChars(NString *string, NUTFType type, const void *chars);
void _NStringInit            (NString *string);
void _NStringDeinit          (NString *string);

nfunc(NString *, NStringCreateWithUTFBytes, (NUTFType type, const void *begin, const void *end));
nfunc(NString *, NStringCreateWithUTFChars, (NUTFType type, const void *chars));
nfunc(NString *, NStringCreate            , (void));

nfunc(NString *, NStringCopy, (NString *that));

nfunc(const char32_t *, NStringU32Chars, (NString *string));
nfunc(const char16_t *, NStringU16Chars, (NString *string));
nfunc(const char     *, NStringU8Chars , (NString *string));

//return the byte size of the string under the specified encoding.
nfunc(int, NStringU32Size, (NString *string));
nfunc(int, NStringU16Size, (NString *string));
nfunc(int, NStringU8Size , (NString *string));

//return the number of the characters from the string.
nfunc(int, NStringLength, (NString *string));

nfunc(NIterator *, NStringRange, (NString *string));

nfunc(bool, NStringIsEmpty, (NString *string));
nfunc(void, NStringAppend , (NString *string, NString *that));
nfunc(int , NStringCompare, (NString *string, NString *that));
