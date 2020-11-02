#include "nunicode.h"

int NStepU32Byte(const void* begin, const void *end, char32_t *outChar) {
    return 0;
}

int NStepU16Byte(const void* begin, const void *end, char32_t *outChar) {
    return 0;
}

int NStepU8Byte(const void* begin, const void *end, char32_t *outChar) {
    return 0;
}

int NStepU32Char(const void* chars, char32_t *outChar) {
    return 0;
}

int NStepU16Char(const void* chars, char32_t *outChar) {
    return 0;
}

int NStepU8Char(const void* chars, char32_t *outChar) {
    return 0;
}

int NCheckU32Bytes(const void *begin, const void *end, int *outLength, int *outMaxCharSize) {
    return 0;
}

int NCheckU16Bytes(const void *begin, const void *end, int *outLength, int *outMaxCharSize) {
    return 0;
}

int NCheckU8Bytes(const void *begin, const void *end, int *outLength, int *outMaxCharSize) {
    return 0;
}

int NCheckU32Chars(const void *chars, int *outLength, int *outMaxCharSize) {
    return 0;
}

int NCheckU16Chars(const void *chars, int *outLength, int *outMaxCharSize) {
    return 0;
}

int NCheckU8Chars(const void *chars, int *outLength, int *outMaxCharSize) {
    return 0;
}

char32_t *NDupU32FromUBytes(NUTFType type, const void *begin, const void *end) {
    return NULL;
}

char16_t *NDupU16FromUBytes(NUTFType type, const void *begin, const void *end) {
    return NULL;
}

char *NDupU8FromUBytes (NUTFType type, const void *begin, const void *end) {
    return NULL;
}

char32_t *NDupU32FromUChars(NUTFType type, const void *chars) {
    return NULL;
}

char16_t *NDupU16FromUChars(NUTFType type, const void *chars) {
    return NULL;
}

char *NDupU8FromUChars (NUTFType type, const void *chars) {
    return NULL;
}
