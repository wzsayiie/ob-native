#include "nunicode.h"
#include "nmemory.h"

int NReadFromU32Bytes(const void* begin, const void *end, char32_t *outChar) {
    const char32_t *ptr = begin;
    const char32_t *lmt = end;

    //4 bytes per character.
    if (ptr + 1 <= lmt) {
        *outChar = *ptr;
        return 4;
    }
    return 0;
}

int NReadFromU16Bytes(const void* begin, const void *end, char32_t *outChar) {
    const char16_t *ptr = begin;
    const char16_t *lmt = end;

    if (ptr + 1 > lmt) {
        *outChar = 0;
        return 0;
    }

    //utf-16 surrogate pair, 4 bytes:
    //
    //  1st 2 bytes: 1101 10 xx,xxxx xxxx
    //  2nd 2 bytes: 1101 11 xx,xxxx xxxx
    //
    if (/*  ptr + 1 <= lmt */ 0xD800 <= ptr[0] && ptr[0] <= 0xDBff) {
        if (ptr + 2 <= lmt && 0xDC00 <= ptr[1] && ptr[1] <= 0xDFff) {

            *outChar = (char32_t)(ptr[0] & 0x03ff);
            *outChar = (char32_t)(ptr[1] & 0x03ff) | (*outChar << 10);
            *outChar += 0x10000;

            return 4;
        } else {
            //error happened.
            *outChar = 0;
            return 0;
        }
    }
    //BMP character, 2 bytes.
    else {
        *outChar = *ptr;
        return 2;
    }
}

int NReadFromU8Bytes(const void* begin, const void *end, char32_t *outChar) {
    const int8_t *ptr = begin;
    const int8_t *lmt = end;

    if (ptr + 1 > lmt) {
        *outChar = 0;
        return 0;
    }

    //utf-8 first byte:
    //
    // 1 byte  occupied,  7 valid bits: 0xxx'xxxx
    // 2 bytes occupied, 11 valid bits: 110x'xxxx 10xx xxxx
    // 3 bytes occupied, 16 valid bits: 1110'xxxx 10xx xxxx ...
    // 4 bytes occupied, 21 valid bits: 1111'0xxx 10xx xxxx ...
    //
    int count = 0;
    if /**/ ((*ptr & 0x80) == 0x00) { count = 1; *outChar = *ptr & 0x7f;}
    else if ((*ptr & 0xE0) == 0xC0) { count = 2; *outChar = *ptr & 0x1f;}
    else if ((*ptr & 0xF0) == 0xE0) { count = 3; *outChar = *ptr & 0x0f;}
    else if ((*ptr & 0xF8) == 0xF0) { count = 4; *outChar = *ptr & 0x07;}

    if (count == 0 || ptr + count > lmt) {
        //error happened.
        *outChar = 0;
        return 0;
    }

    //followed bytes: 10xx'xxxx
    for (const int8_t *it = ptr + 1; it < ptr + count; ++it) {
        if ((*it & 0xC0) != 0x80) {
            //error happened.
            *outChar = 0;
            return 0;
        }
        *outChar <<= 6;
        *outChar |= (*it & 0x3f);
    }

    return count;
}

int NReadFromU32Chars(const void* p, char32_t *c) {int s = NReadFromU32Bytes(p, (int8_t *)p + 4, c); return *c ? s : 0;}
int NReadFromU16Chars(const void* p, char32_t *c) {int s = NReadFromU16Bytes(p, (int8_t *)p + 4, c); return *c ? s : 0;}
int NReadFromU8Chars (const void* p, char32_t *c) {int s = NReadFromU8Bytes (p, (int8_t *)p + 4, c); return *c ? s : 0;}

int NWriteU32(void *dst, char32_t chr) {
    char32_t *ptr = dst;

    //4 bytes always.
    *ptr = chr;
    return 4;
}

int NWriteU16(void *dst, char32_t chr) {
    char16_t *ptr = dst;

    //utf-16 surrogate pair, 4 bytes:
    //
    //  1st 2 bytes: 1101 10 xx,xxxx xxxx
    //  2nd 2 bytes: 1101 11 xx,xxxx xxxx
    //
    if (chr > 0xFFff) {

        //the biggest code-point of unicode is 0x10FFff (the last of 16st plane),
        //when a code-point subtracted from 0x10000, the remaining value will not exceed 20 bit.
        chr -= 0x10000;
        ptr[0] = ((chr >> 10) & 0x3ff) | 0xD800;
        ptr[1] = ((chr      ) & 0x3ff) | 0xDC00;

        return 4;
    }
    //BMP character, 2 bytes.
    else {
        ptr[0] = (char16_t)chr;
        return 2;
    }
}

int NWriteU8(void *dst, char32_t chr) {
    int8_t *ptr = dst;

    if (chr <= 0x7f) {
        //up to 7 bits, occupy 1 byte. 0xxx'xxxx.
        ptr[0] = (int8_t)chr;

        return 1;
    }
    if (chr <= 0x7ff) {
        //up to 11 bits, occupy 2 bytes. 110x'xxxx 10xx xxxx.
        ptr[0] = ((chr >> 6) & 0x1f) | 0xC0;
        ptr[1] = ((chr     ) & 0x3f) | 0x80;

        return 2;
    }
    if (chr <= 0xFFff) {
        //up to 16 bit, occupy 3 bytes. 1110'xxxx 10xx xxxx ... .
        ptr[0] = ((chr >> 12) & 0x0f) | 0xE0;
        ptr[1] = ((chr >>  6) & 0x3f) | 0x80;
        ptr[2] = ((chr      ) & 0x3f) | 0x80;

        return 3;
    }
    if (chr <= 0x10FFff) {
        //up to 21 bits, occupy 4 bytes. 1111'0xxx 10xx xxxx ... .
        ptr[0] = ((chr >> 18) & 0x07) | 0xF0;
        ptr[1] = ((chr >> 12) & 0x3f) | 0x80;
        ptr[2] = ((chr >>  6) & 0x3f) | 0x80;
        ptr[3] = ((chr      ) & 0x3f) | 0x80;

        return 4;
    }

    return 0;
}

int NUTFBytesSize(NUTFType type, const NUTFCharsStat *s) {
    if (!s) {
        return 0;
    }
    
    int b7  = (int)s->bit7 ;
    int b11 = (int)s->bit11;
    int b16 = (int)s->bit16;
    int b21 = (int)s->bit21;
    
    switch (type) {
    case NUTF32: return b7 * 4 + b11 * 4 + b16 * 4 + b21 * 4;
    case NUTF16: return b7 * 2 + b11 * 2 + b16 * 2 + b21 * 4;
    case NUTF8 : return b7 * 1 + b11 * 2 + b16 * 3 + b21 * 4;
    default    : return 0;
    }
}

int NUTFCharsCount(const NUTFCharsStat *s) {
    if (s) {
        return s->bit7 + s->bit11 + s->bit16 + s->bit21;
    }
    return 0;
}

typedef int (*UTFReader)(const void *, const void *, char32_t *);
typedef int (*UTFWriter)(void *, char32_t);

static int ReadFromU32Chars(const void *s, const void *_, char32_t *c) {return NReadFromU32Chars(s, c);}
static int ReadFromU16Chars(const void *s, const void *_, char32_t *c) {return NReadFromU16Chars(s, c);}
static int ReadFromU8Chars (const void *s, const void *_, char32_t *c) {return NReadFromU8Chars (s, c);}

static int CheckUTF(NUTFType type, bool isBytes, const void *p1, const void *p2, NUTFCharsStat *outStat) {

    //select a reader.
    UTFReader reader = NULL;
    if /**/ (type == NUTF32) {reader = isBytes ? NReadFromU32Bytes : ReadFromU32Chars;}
    else if (type == NUTF16) {reader = isBytes ? NReadFromU16Bytes : ReadFromU16Chars;}
    else if (type == NUTF8 ) {reader = isBytes ? NReadFromU8Bytes  : ReadFromU8Chars ;}

    if (!reader || !p1) {
        NZeroMemory(outStat, nisizeof(*outStat));
        return 0;
    }

    //read data.
    NUTFCharsStat stat = {0};
    int totalSize = 0;

    while (true) {
        char32_t chr = 0;
        int size = reader(p1, p2, &chr);
        if (size <= 0) {
            break;
        }

        p1 = (int8_t *)p1 + size;
        totalSize += size;

        if /**/ (chr <= 0x00007F) {stat.bit7  += 1;}
        else if (chr <= 0x0007FF) {stat.bit11 += 1;}
        else if (chr <= 0x00ffFF) {stat.bit16 += 1;}
        else if (chr <= 0x10ffFF) {stat.bit21 += 1;}
    }

    if (outStat) {
        *outStat = stat;
    }
    return totalSize;
}

int NCheckUTFBytes(NUTFType t, const void *b, const void *e, NUTFCharsStat *s) {return CheckUTF(t, 1, b, e, s);}
int NCheckUTFChars(NUTFType t, const void *c, /*   NULL   */ NUTFCharsStat *s) {return CheckUTF(t, 0, c, 0, s);}

static void *DupUTF(NUTFType dstType, NUTFType srcType, bool isBytes, const void *p1, const void *p2) {

    //select a reader and a writer.
    UTFReader reader = NULL;
    if /**/ (srcType == NUTF32) {reader = isBytes ? NReadFromU32Bytes : ReadFromU32Chars;}
    else if (srcType == NUTF16) {reader = isBytes ? NReadFromU16Bytes : ReadFromU16Chars;}
    else if (srcType == NUTF8 ) {reader = isBytes ? NReadFromU8Bytes  : ReadFromU8Chars ;}

    UTFWriter writer = NULL;
    if /**/ (dstType == NUTF32) {writer = NWriteU32;}
    else if (dstType == NUTF16) {writer = NWriteU16;}
    else if (dstType == NUTF8 ) {writer = NWriteU8 ;}

    if (!reader || !writer || !p1) {
        return NULL;
    }

    //copy data.
    int8_t *buff = NULL;
    int buffSize = 0;
    int usedSize = 0;

    while (true) {
        char32_t chr = 0;
        int size = reader(p1, p2, &chr);
        if (size <= 0) {
            break;
        }

        p1 = (int8_t *)p1 + size;

        if (usedSize + 4 > buffSize) {
            buffSize += 256; //allocate 256 bytes one time.
            buff = NReallocMemory(buff, buffSize);
        }
        usedSize += writer(buff + usedSize, chr);
    }

    //add '\0' at the end.
    if (usedSize + 4 > buffSize) {
        buffSize += 4;
        buff = NReallocMemory(buff, buffSize);
    }
    usedSize += writer(buff + usedSize, 0);

    buff = NReallocMemory(buff, usedSize);
    return buff;
}

char32_t *NDupU32FromBytes(NUTFType t, const void *b, const void *e) {return DupUTF(NUTF32, t, 1, b, e);}
char16_t *NDupU16FromBytes(NUTFType t, const void *b, const void *e) {return DupUTF(NUTF16, t, 1, b, e);}
char     *NDupU8FromBytes (NUTFType t, const void *b, const void *e) {return DupUTF(NUTF8 , t, 1, b, e);}

char32_t *NDupU32FromChars(NUTFType t, const void *c) {return DupUTF(NUTF32, t, 0, c, 0);}
char16_t *NDupU16FromChars(NUTFType t, const void *c) {return DupUTF(NUTF16, t, 0, c, 0);}
char     *NDupU8FromChars (NUTFType t, const void *c) {return DupUTF(NUTF8 , t, 0, c, 0);}
