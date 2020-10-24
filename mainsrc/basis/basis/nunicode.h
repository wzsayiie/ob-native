#pragma once

#include "ndef.h"

nenum(NUTFType) {
    NUTF8  = 1,
    NUTF16 = 2,
    NUTF32 = 3,
};

int NStepU32Byte(const void* begin, const void *end, char32_t *codePoint);
int NStepU16Byte(const void* begin, const void *end, char32_t *codePoint);
int NStepU8Byte (const void* begin, const void *end, char32_t *codePoint);

int NStepU32Char(const void* chars, char32_t *codePoint);
int NStepU16Char(const void* chars, char32_t *codePoint);
int NStepU8Char (const void* chars, char32_t *codePoint);

int NCheckU32Bytes(const void *begin, const void *end, int *maxCharSize);
int NCheckU16Bytes(const void *begin, const void *end, int *maxCharSize);
int NCheckU8Bytes (const void *begin, const void *end, int *maxCharSize);

int NCheckU32Chars(const void *chars, int *maxCharSize);
int NCheckU16Chars(const void *chars, int *maxCharSize);
int NCheckU8Chars (const void *chars, int *maxCharSize);

char32_t *NDupU32FromUBytes(NUTFType type, const void *begin, const void *end);
char16_t *NDupU16FromUBytes(NUTFType type, const void *begin, const void *end);
char     *NDupU8FromUBytes (NUTFType type, const void *begin, const void *end);
char32_t *NDupU32FromUChars(NUTFType type, const void *chars);
char16_t *NDupU16FromUChars(NUTFType type, const void *chars);
char     *NDupU8FromUChars (NUTFType type, const void *chars);
