#pragma once

#include "nobject.h"

nstruct(NString);

NString *NStringCreateU32(const char32_t *raw);
NString *NStringCreateU16(const char16_t *raw);
NString *NStringCreateU8 (const char     *raw);
NString *NStringCreate   (void);

const char32_t *NStringU32Chars(NString *self);
const char16_t *NStringU16Chars(NString *self);
const char     *NStringU8Chars (NString *self);

//return the byte size of the string under the specified encoding.
int NStringU32Size(NString *self);
int NStringU16Size(NString *self);
int NStringU8Size (NString *self);

//return the number of the characters from the string.
int NStringLength(NString *self);

char32_t NStringCharAt(NString *self, int index);

void NStringAssign (NString *self, NString *that);
void NStringAppend (NString *self, NString *that);
int  NStringCompare(NString *self, NString *that);
