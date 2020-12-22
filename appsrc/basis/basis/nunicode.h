#pragma once

#include "nenviron.h"

nfunc(int, NReadFromU32Bytes, (const void *nonnull_begin, const void *end, char32_t *nonnull_outChar));
nfunc(int, NReadFromU16Bytes, (const void *nonnull_begin, const void *end, char32_t *nonnull_outChar));
nfunc(int, NReadFromU8Bytes , (const void *nonnull_begin, const void *end, char32_t *nonnull_outChar));

nfunc(int, NReadFromU32Chars, (const void *nonnull_chars, char32_t *nonnull_outChar));
nfunc(int, NReadFromU16Chars, (const void *nonnull_chars, char32_t *nonnull_outChar));
nfunc(int, NReadFromU8Chars , (const void *nonnull_chars, char32_t *nonnull_outChar));

nfunc(int, NWriteU32, (void *nonnull_dst, char32_t chr));
nfunc(int, NWriteU16, (void *nonnull_dst, char32_t chr));
nfunc(int, NWriteU8 , (void *nonnull_dst, char32_t chr));

nenum(NUTFType) {
    NUTF8  = 1,
    NUTF16 = 2,
    NUTF32 = 3,
};

nstruct(NUTFCharsStat) {
    npint bit7 ; //the count of characters occupying up to 7 bits.
    npint bit11; //ditto.
    npint bit16;
    npint bit21;
};

nfunc(int, NUTFBytesSize , (NUTFType type, const NUTFCharsStat *stat));
nfunc(int, NUTFCharsCount, (const NUTFCharsStat *stat));

nfunc(int, NCheckUTFBytes, (NUTFType type, const void *begin, const void *end, NUTFCharsStat *outStat));
nfunc(int, NCheckUTFChars, (NUTFType type, const void *chars, NUTFCharsStat *outStat));

nfunc(char32_t *, NDupU32FromBytes, (NUTFType type, const void *begin, const void *end));
nfunc(char16_t *, NDupU16FromBytes, (NUTFType type, const void *begin, const void *end));
nfunc(char     *, NDupU8FromBytes , (NUTFType type, const void *begin, const void *end));

nfunc(char32_t *, NDupU32FromChars, (NUTFType type, const void *chars));
nfunc(char16_t *, NDupU16FromChars, (NUTFType type, const void *chars));
nfunc(char     *, NDupU8FromChars , (NUTFType type, const void *chars));
