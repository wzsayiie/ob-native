#include "nstring.h"

void _NStringInitWithUTFBytes(NString *string, NUTFType type, const void *begin, const void *end) {
    _NObjectInit(nsuperof(string));

    string->length = -1;

    if /**/ (type == NUTF32) {string->u32chars = NDupU32FromBytes(type, begin, end);}
    else if (type == NUTF16) {string->u16chars = NDupU16FromBytes(type, begin, end);}
    else if (type == NUTF8 ) {string->u8chars  = NDupU8FromBytes (type, begin, end);}
}

void _NStringInitWithUTFChars(NString *string, NUTFType type, const void *chars) {
    _NObjectInit(nsuperof(string));

    string->length = -1;

    if /**/ (type == NUTF32) {string->u32chars = NDupU32FromChars(type, chars);}
    else if (type == NUTF16) {string->u16chars = NDupU16FromChars(type, chars);}
    else if (type == NUTF8 ) {string->u8chars  = NDupU8FromChars (type, chars);}
}

void _NStringInit(NString *string) {
    _NObjectInit(nsuperof(string));

    string->length = -1;
}

void _NStringDeinit(NString *string) {
    NFreeMemory(string->u32chars);
    NFreeMemory(string->u16chars);
    NFreeMemory(string->u8chars );
    
    _NObjectDeinit(nsuperof(string));
}

NString *NStringCreateWithUTFBytes(NUTFType type, const void *begin, const void *end) {
    NString *string = NAlloc(NString, _NStringDeinit);
    _NStringInitWithUTFBytes(string, type, begin, end);
    return string;
}

NString *NStringCreateWithUTFChars(NUTFType type, const void *chars) {
    NString *string = NAlloc(NString, _NStringDeinit);
    _NStringInitWithUTFChars(string, type, chars);
    return string;
}

NString *NStringCreate(void) {
    NString *string = NAlloc(NString, _NStringDeinit);
    _NStringInit(string);
    return string;
}

NString *NStringCopy(NString *that) {
    if (!that) {
        return NULL;
    }

    NString *string = NStringCreate();

    string->length = that->length;
    //only copy one format.
    if /**/ (that->u32chars) {string->u32chars = NDupMemory(that->u32chars);}
    else if (that->u16chars) {string->u16chars = NDupMemory(that->u16chars);}
    else if (that->u8chars ) {string->u8chars  = NDupMemory(that->u8chars );}

    return string;
}

static const void *StringTryMake(NString *s, NUTFType type) {
    if (type == NUTF32) {
        if (!s->u32chars) {
            if /**/ (s->u16chars) {s->u32chars = NDupU32FromChars(NUTF16, s->u16chars);}
            else if (s->u8chars ) {s->u32chars = NDupU32FromChars(NUTF8 , s->u8chars );}
        }
        return s->u32chars;
    }
    if (type == NUTF16) {
        if (!s->u16chars) {
            if /**/ (s->u32chars) {s->u16chars = NDupU16FromChars(NUTF32, s->u32chars);}
            else if (s->u8chars ) {s->u16chars = NDupU16FromChars(NUTF8 , s->u8chars );}
        }
        return s->u16chars;
    }
    if (type == NUTF8) {
        if (!s->u8chars) {
            if /**/ (s->u32chars) {s->u8chars = NDupU8FromChars(NUTF32, s->u32chars);}
            else if (s->u16chars) {s->u8chars = NDupU8FromChars(NUTF16, s->u16chars);}
        }
        return s->u8chars;
    }
    return NULL;
}

const void *StringUTFChars(NString *string, NUTFType type, const void *defaults) {
    if (string) {
        const void *chars = StringTryMake(string, type);
        return chars ? chars : defaults;
    }
    return defaults;
}

const char32_t *NStringU32Chars(NString *s) {return StringUTFChars(s, NUTF32, U"" );}
const char16_t *NStringU16Chars(NString *s) {return StringUTFChars(s, NUTF16, u"" );}
const char     *NStringU8Chars (NString *s) {return StringUTFChars(s, NUTF8 , u8"");}

static int StringUTFSize(NString *string, NUTFType type) {
    if (!string) {
        return 0;
    }

    const void *chars = StringTryMake(string, type);
    if (chars) {
        int size = NMemorySize(chars);

        if (type == NUTF32) {return size - nsizeof(char32_t);}
        if (type == NUTF16) {return size - nsizeof(char16_t);}
        if (type == NUTF8 ) {return size - nsizeof(char    );}

        return 0;
    }
    return 0;
}

int NStringU32Size(NString *s) {return StringUTFSize(s, NUTF32);}
int NStringU16Size(NString *s) {return StringUTFSize(s, NUTF16);}
int NStringU8Size (NString *s) {return StringUTFSize(s, NUTF8 );}

int NStringLength(NString *string) {
    if (!string) {
        return 0;
    }

    if (string->length >= 0) {
        return string->length;
    }

    NUTFCharsStat stat = {0};
    if /**/ (string->u32chars) {NCheckUTFChars(NUTF32, string->u32chars, &stat);}
    else if (string->u16chars) {NCheckUTFChars(NUTF16, string->u16chars, &stat);}
    else if (string->u8chars ) {NCheckUTFChars(NUTF8 , string->u8chars , &stat);}

    string->length = NUTFCharsCount(&stat);
    return string->length;
}

nstruct(StringIterator, {
    NIterator super;

    int (*step)(const void *, char32_t *);
    char32_t current;
    void *remaining;
});

static bool StringIteratorNext(StringIterator *it) {
    int size = it->step(it->remaining, &it->current);
    if (size > 0) {
        it->remaining = (uint8_t *)it->remaining + size;
        return true;
    }
    return false;
}

static void *StringIteratorCurr(StringIterator *it) {
    return &it->current;
}

NIterator *NStringRange(NString *string) {
    if (NStringIsEmpty(string)) {
        return NStoreIterator(NULL, 0);
    }

    StringIterator it = {0};
    it.super.next = (NIteratorNext)StringIteratorNext;
    it.super.curr = (NIteratorCurr)StringIteratorCurr;

    if /**/ (string->u32chars) {it.step = NReadFromU32Chars; it.remaining = string->u32chars;}
    else if (string->u16chars) {it.step = NReadFromU16Chars; it.remaining = string->u16chars;}
    else if (string->u8chars ) {it.step = NReadFromU8Chars ; it.remaining = string->u8chars ;}

    return NStoreIterator(&it, sizeof(it));
}

bool NStringIsEmpty(NString *string) {
    if (string) {
        return (!string->u32chars
        /**/ && !string->u16chars
        /**/ && !string->u8chars);
    } else {
        return true;
    }
}

static void StringJoin(void **head, void *tail, int zeroSize) {
    if (*head) {
        int headSize = NMemorySize(*head);
        int tailSize = NMemorySize( tail);

        *head = NReallocMemory(*head, headSize - zeroSize + tailSize);
        NMoveMemory((int8_t *)*head + headSize - zeroSize, tail, tailSize);

    } else {
        *head = NDupMemory(tail);
    }
}

static void StringOnlyReserve(NString *string, NUTFType type) {
    if (type != NUTF32) {NFreeMemory(string->u32chars); string->u32chars = NULL;}
    if (type != NUTF16) {NFreeMemory(string->u16chars); string->u16chars = NULL;}
    if (type != NUTF8 ) {NFreeMemory(string->u8chars ); string->u8chars  = NULL;}
}

void NStringAppend(NString *string, NString *that) {
    if (!string || !that) {
        return;
    }

    NUTFType type = 0;

    if /**/ (string->u32chars && that->u32chars) {type = NUTF32;}
    else if (string->u16chars && that->u16chars) {type = NUTF16;}
    else if (string->u8chars  && that->u8chars ) {type = NUTF8 ;}

    else if (that->u32chars) {type = NUTF32; StringTryMake(string, NUTF32);}
    else if (that->u16chars) {type = NUTF16; StringTryMake(string, NUTF16);}
    else if (that->u8chars ) {type = NUTF8 ; StringTryMake(string, NUTF8 );}

    if /**/ (type == NUTF32) {StringJoin((void **)&string->u32chars, that->u32chars, 4);}
    else if (type == NUTF16) {StringJoin((void **)&string->u16chars, that->u16chars, 2);}
    else if (type == NUTF8 ) {StringJoin((void **)&string->u8chars , that->u8chars , 1);}

    StringOnlyReserve(string, type);
    string->length = -1; //invalidate length.
}

int NStringCompare(NString *string, NString *that) {
    if (string == that) {
        return 0;
    }
    
    NIterator *thisIter = NStringRange(string);
    NIterator *thatIter = NStringRange(that);
    while (true) {
        bool thisValid = thisIter->next(thisIter);
        bool thatValid = thatIter->next(thatIter);

        if (thisValid && thatValid) {
            char32_t thisChar = thisIter->curr(thisIter).asUInt32;
            char32_t thatChar = thatIter->curr(thatIter).asUInt32;

            if (thisChar > thatChar) {return  1;}
            if (thisChar < thatChar) {return -1;}

        } else if (thisValid) {
            return 1;

        } else if (thatValid) {
            return -1;

        } else {
            return 0;
        }
    }
}
