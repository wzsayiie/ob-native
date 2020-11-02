#include "nstring.h"
#include "nmemory.h"

nstruct(NStringIterator) {
    NIterator super;

    int    (*step)(const void *, char32_t *);
    char32_t head;
    void    *back;
};

nstruct(NString) {
    int       length  ;
    char32_t *u32chars;
    char16_t *u16chars;
    char     *u8chars ;

    NStringIterator iterator;
};

static void _NStringClear(NString *string) {
    NFree(string->u32chars);
    NFree(string->u16chars);
    NFree(string->u8chars );
}

NString *NStringCreateWithUBytes(NUTFType type, const void *begin, const void *end) {
    NString *self = NCreate(nisizeof(NString), _NStringClear);

    int maxCharSize = 0;
    switch (type) {
    case NUTF32:
        NCheckU32Bytes(begin, end, &self->length, &maxCharSize);
        self->u32chars = NDupU32FromUBytes(NUTF32, begin, end);
        break;
    case NUTF16:
        NCheckU16Bytes(begin, end, &self->length, &maxCharSize);
        switch (maxCharSize) {
        case 4: self->u32chars = NDupU32FromUBytes(NUTF16, begin, end); break;
        case 2: self->u16chars = NDupU16FromUBytes(NUTF16, begin, end); break;
        default:;
        }
        break;
    case NUTF8:
        NCheckU8Bytes(begin, end, &self->length, &maxCharSize);
        switch (maxCharSize) {
        case 4: self->u32chars = NDupU32FromUBytes(NUTF8, begin, end); break;
        case 3: self->u16chars = NDupU16FromUBytes(NUTF8, begin, end); break;
        case 2: self->u16chars = NDupU16FromUBytes(NUTF8, begin, end); break;
        case 1: self->u8chars  = NDupU8FromUBytes (NUTF8, begin, end); break;
        default:;
        }
        break;
    default:;
    }

    return self;
}

NString *NStringCreateWithUChars(NUTFType type, const void *chars) {
    NString *self = NCreate(nisizeof(NString), _NStringClear);

    int maxCharSize = 0;
    switch (type) {
    case NUTF32:
        NCheckU32Chars(chars, &self->length, &maxCharSize);
        self->u32chars = NDupU32FromUChars(NUTF32, chars);
        break;
    case NUTF16:
        NCheckU16Chars(chars, &self->length, &maxCharSize);
        switch (maxCharSize) {
        case 4: self->u32chars = NDupU32FromUChars(NUTF16, chars); break;
        case 2: self->u16chars = NDupU16FromUChars(NUTF16, chars); break;
        default:;
        }
        break;
    case NUTF8:
        NCheckU8Chars(chars, &self->length, &maxCharSize);
        switch (maxCharSize) {
        case 4: self->u32chars = NDupU32FromUChars(NUTF8, chars); break;
        case 3: self->u16chars = NDupU16FromUChars(NUTF8, chars); break;
        case 2: self->u16chars = NDupU16FromUChars(NUTF8, chars); break;
        case 1: self->u8chars  = NDupU8FromUChars (NUTF8, chars); break;
        default:;
        }
        break;
    default:;
    }

    return self;
}

NString *NStringCreate(void) {
    return NCreate(nisizeof(NString), _NStringClear);
}

NString *NStringCopy(NString *that) {
    if (!that) {
        return NULL;
    }

    NString *self = NStringCreate();

    self->length = that->length;

    //only copy one format.
    /**/ if (that->u32chars) {self->u32chars = NDup(that->u32chars);}
    else if (that->u16chars) {self->u16chars = NDup(that->u16chars);}
    else if (that->u8chars ) {self->u8chars  = NDup(that->u8chars );}

    return self;
}

static bool _NStringTryMake(NString *s, NUTFType type) {
    if (type == NUTF32 && !s->u32chars) {
        /**/ if (s->u16chars) {s->u32chars = NDupU32FromUChars(NUTF16, s->u16chars);}
        else if (s->u8chars ) {s->u32chars = NDupU32FromUChars(NUTF8 , s->u8chars );}

        return s->u32chars;
    }
    if (type == NUTF16 && !s->u16chars) {
        /**/ if (s->u32chars) {s->u16chars = NDupU16FromUChars(NUTF32, s->u32chars);}
        else if (s->u8chars ) {s->u16chars = NDupU16FromUChars(NUTF8 , s->u8chars );}

        return s->u16chars;
    }
    if (type == NUTF8 && !s->u8chars) {
        /**/ if (s->u32chars) {s->u8chars = NDupU8FromUChars(NUTF32, s->u32chars);}
        else if (s->u16chars) {s->u8chars = NDupU8FromUChars(NUTF16, s->u16chars);}

        return s->u8chars;
    }
    return false;
}

const char32_t *NStringU32Chars(NString *self) {
    if (!self) {
        return NULL;
    }

    if (_NStringTryMake(self, NUTF32)) {
        return self->u32chars;
    } else {
        return U"\0";
    }
}

const char16_t *NStringU16Chars(NString *self) {
    if (!self) {
        return NULL;
    }

    if (_NStringTryMake(self, NUTF16)) {
        return self->u16chars;
    } else {
        return u"\0";
    }
}

const char *NStringU8Chars(NString *self) {
    if (!self) {
        return NULL;
    }

    if (_NStringTryMake(self, NUTF8)) {
        return self->u8chars;
    } else {
        return u8"\0";
    }
}

static int _NStringUCharsSize(NString *string, NUTFType type) {
    if (type == NUTF32 && string->u32chars) {
        int size = NMemorySize(string->u32chars);
        return size - nisizeof(char32_t);
    }
    if (type == NUTF16 && string->u16chars) {
        int size = NMemorySize(string->u16chars);
        return size - nisizeof(char16_t);
    }
    if (type == NUTF8 && string->u8chars) {
        int size = NMemorySize(string->u8chars);
        return size - nisizeof(char);
    }
    return 0;
}

int NStringU32Size(NString *self) {
    if (self) {
        _NStringTryMake(self, NUTF32);
        return _NStringUCharsSize(self, NUTF32);
    } else {
        return 0;
    }
}

int NStringU16Size(NString *self) {
    if (self) {
        _NStringTryMake(self, NUTF16);
        return _NStringUCharsSize(self, NUTF16);
    } else {
        return 0;
    }
}

int NStringU8Size(NString *self) {
    if (self) {
        _NStringTryMake(self, NUTF8);
        return _NStringUCharsSize(self, NUTF8);
    } else {
        return 0;
    }
}

int NStringLength(NString *self) {
    if (!self) {
        return 0;
    }

    if (self->length < 0) {
        if (self->u32chars) {
            //firstly try to get the size of "u32chars".
            int size = _NStringUCharsSize(self, NUTF32);
            self->length = size / nisizeof(char32_t);
        } else {
            int length = 0;
            nfor(char32_t, chr, NStringRange(self)) {
                length += 1;
            }
            self->length = length;
        }
    }
    return self->length;
}

static bool _NStringNext(NStringIterator *iterator) {
    int size = iterator->step(iterator->back, &iterator->head);
    if (size > 0 && iterator->head != U'\0') {
        iterator->back = (char *)iterator->back + size;
        return true;
    }
    return false;
}

static void *_NStringGet(NStringIterator *iterator) {
    return &iterator->head;
}

NIterator *NStringRange(NString *self) {
    if (NStringIsEmpty(self)) {
        return NIteratorGetEmpty();
    }

    NStringIterator *iter = &self->iterator;
    NZeroMemory(iter, nisizeof(NStringIterator));
    iter->super.Next = (bool  (*)(NIterator *))_NStringNext;
    iter->super.Get  = (void *(*)(NIterator *))_NStringGet ;

    /**/ if (self->u32chars) {iter->step = NStepU32Char; iter->back = self->u32chars;}
    else if (self->u16chars) {iter->step = NStepU16Char; iter->back = self->u16chars;}
    else if (self->u8chars ) {iter->step = NStepU8Char ; iter->back = self->u8chars ;}

    return &iter->super;
}

bool NStringIsEmpty(NString *self) {
    if (self) {
        return (!self->u32chars
        /**/ && !self->u16chars
        /**/ && !self->u8chars);
    } else {
        return true;
    }
}

static void _NStringJoin(void **head, void *tail, int zeroSize) {
    if (*head) {
        int headSize = NMemorySize(*head);
        int tailSize = NMemorySize( tail);

        NRealloc(*head, headSize - zeroSize + tailSize);
        NMoveMemory((char *)*head + headSize - zeroSize, tail, tailSize);

    } else {
        *head = NDup(tail);
    }
}

static void _NStringOnlyReserve(NString *string, NUTFType type) {
    if (type != NUTF32) {NFree(string->u32chars); string->u32chars = NULL;}
    if (type != NUTF16) {NFree(string->u16chars); string->u16chars = NULL;}
    if (type != NUTF8 ) {NFree(string->u8chars ); string->u8chars  = NULL;}
}

void NStringAppend(NString *self, NString *that) {
    if (!self || !that) {
        return;
    }

    NUTFType type = 0;

    //try not to generate new format.
    /**/ if (self->u32chars && that->u32chars) {type = NUTF32;}
    else if (self->u16chars && that->u16chars) {type = NUTF16;}
    else if (self->u8chars  && that->u8chars ) {type = NUTF8 ;}

    else if (that->u32chars) {type = NUTF32; _NStringTryMake(self, NUTF32);}
    else if (that->u16chars) {type = NUTF16; _NStringTryMake(self, NUTF16);}
    else if (that->u8chars ) {type = NUTF8 ; _NStringTryMake(self, NUTF8 );}

    switch(type) {
    case NUTF32: _NStringJoin(&self->u32chars, that->u32chars, 4); break;
    case NUTF16: _NStringJoin(&self->u16chars, that->u16chars, 2); break;
    case NUTF8 : _NStringJoin(&self->u8chars , that->u8chars , 1); break;
    default:;
    }
    _NStringOnlyReserve(self, type);
    self->length = -1; //invalidate length.
}

int NStringCompare(NString *self, NString *that) {
    NIterator *selfIter = NStringRange(self);
    NIterator *thatIter = NStringRange(that);

    while (true) {
        bool selfValid = selfIter->Next(selfIter);
        bool thatValid = thatIter->Next(thatIter);

        if (selfValid && thatValid) {
            char32_t selfChar = *(char32_t *)selfIter->Get(selfIter);
            char32_t thatChar = *(char32_t *)thatIter->Get(thatIter);

            if (selfChar > thatChar) {return  1;}
            if (selfChar < thatChar) {return -1;}

        } else if (selfValid) {
            return 1;

        } else if (thatValid) {
            return -1;

        } else {
            return 0;
        }
    }
}
