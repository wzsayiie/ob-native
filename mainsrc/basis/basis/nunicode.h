#pragma once

#include "ndef.h"

int NReadFromU32Bytes(const void *nonnull_begin, const void *end, char32_t *nonnull_outChar);
int NReadFromU16Bytes(const void *nonnull_begin, const void *end, char32_t *nonnull_outChar);
int NReadFromU8Bytes (const void *nonnull_begin, const void *end, char32_t *nonnull_outChar);

int NReadFromU32Chars(const void *nonnull_chars, char32_t *nonnull_outChar);
int NReadFromU16Chars(const void *nonnull_chars, char32_t *nonnull_outChar);
int NReadFromU8Chars (const void *nonnull_chars, char32_t *nonnull_outChar);

int NWriteU32(void *nonnull_dst, char32_t chr);
int NWriteU16(void *nonnull_dst, char32_t chr);
int NWriteU8 (void *nonnull_dst, char32_t chr);

nenum(NUTFType) {
    NUTF8  = 1,
    NUTF16 = 2,
    NUTF32 = 3,
};

nstruct(NUTFCharsStat) {
    int bit7 ; //the count of characters occupying up to 7 bits.
    int bit11; //ditto.
    int bit16;
    int bit21;
};

int NUTFBytesSize(NUTFType type, const NUTFCharsStat *stat);
int NUTFCharsCount(const NUTFCharsStat *stat);

int NCheckUTFBytes(NUTFType type, const void *begin, const void *end, NUTFCharsStat *outStat);
int NCheckUTFChars(NUTFType type, const void *chars, NUTFCharsStat *outStat);

char32_t *NDupU32FromBytes(NUTFType type, const void *begin, const void *end);
char16_t *NDupU16FromBytes(NUTFType type, const void *begin, const void *end);
char     *NDupU8FromBytes (NUTFType type, const void *begin, const void *end);

char32_t *NDupU32FromChars(NUTFType type, const void *chars);
char16_t *NDupU16FromChars(NUTFType type, const void *chars);
char     *NDupU8FromChars (NUTFType type, const void *chars);
