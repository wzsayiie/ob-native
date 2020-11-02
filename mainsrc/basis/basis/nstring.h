#pragma once

#include "niterator.h"
#include "nobject.h"
#include "nunicode.h"

nstruct(NString);

NString *NStringCreateWithUBytes(NUTFType type, const void *begin, const void *end);
NString *NStringCreateWithUChars(NUTFType type, const void *chars);
NString *NStringCreate          (void);

NString *NStringCopy(NString *that);

const char32_t *NStringU32Chars(NString *self);
const char16_t *NStringU16Chars(NString *self);
const char     *NStringU8Chars (NString *self);

//return the byte size of the string under the specified encoding.
int NStringU32Size(NString *self);
int NStringU16Size(NString *self);
int NStringU8Size (NString *self);

//return the number of the characters from the string.
int NStringLength(NString *self);

NIterator *NStringRange(NString *self);

bool NStringIsEmpty(NString *self);
void NStringAppend (NString *self, NString *that);
int  NStringCompare(NString *self, NString *that);
