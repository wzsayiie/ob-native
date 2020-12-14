#pragma once

#include "niterator.h"
#include "nmemory.h"
#include "nunicode.h"

nstruct(NString);

nfunc(NString *, NStringCreateWithUTFBytes, (NUTFType type, const void *begin, const void *end));
nfunc(NString *, NStringCreateWithUTFChars, (NUTFType type, const void *chars));
nfunc(NString *, NStringCreate            , (void));

nfunc(NString *, NStringCopy, (NString *that));

nfunc(const char32_t *, NStringU32Chars, (NString *self));
nfunc(const char16_t *, NStringU16Chars, (NString *self));
nfunc(const char     *, NStringU8Chars , (NString *self));

//return the byte size of the string under the specified encoding.
nfunc(int, NStringU32Size, (NString *self));
nfunc(int, NStringU16Size, (NString *self));
nfunc(int, NStringU8Size , (NString *self));

//return the number of the characters from the string.
nfunc(int, NStringLength, (NString *self));

nfunc(NIterator *, NStringRange, (NString *self));

nfunc(bool, NStringIsEmpty, (NString *self));
nfunc(void, NStringAppend , (NString *self, NString *that));
nfunc(int , NStringCompare, (NString *self, NString *that));
