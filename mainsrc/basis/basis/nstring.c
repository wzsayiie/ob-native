#include "nstring.h"
#include "nmemory.h"

nstruct(NString) {
    char32_t *u32chars;
    char16_t *u16chars;
    char     *u8chars ;
};

static NUTFType NStringPreferredUTF(NString *self) {
    if (self->u32chars) {return NUTF32;}
    if (self->u16chars) {return NUTF16;}
    if (self->u8chars ) {return NUTF8 ;}

    return 0;
}

static bool NStringTryMake(NString *self, NUTFType type) {
    if (type == NUTF32 && !self->u32chars) {
        /**/ if (self->u16chars) {self->u32chars = NDupU32FromUChars(NUTF16, self->u16chars);}
        else if (self->u8chars ) {self->u32chars = NDupU32FromUChars(NUTF8 , self->u8chars );}

    } else if (type == NUTF16 && !self->u16chars) {
        /**/ if (self->u32chars) {self->u16chars = NDupU16FromUChars(NUTF32, self->u32chars);}
        else if (self->u8chars ) {self->u16chars = NDupU16FromUChars(NUTF8 , self->u8chars );}

    } else if (type == NUTF8 && !self->u8chars) {
        /**/ if (self->u32chars) {self->u8chars = NDupU8FromUChars(NUTF32, self->u32chars);}
        else if (self->u16chars) {self->u8chars = NDupU8FromUChars(NUTF16, self->u16chars);}
    }

    switch (type) {
    case NUTF32: return self->u32chars;
    case NUTF16: return self->u16chars;
    case NUTF8 : return self->u8chars ;
    default    : return false;
    }
}

static void NStringDeinit(NString *self) {
    NFree(self->u32chars);
    NFree(self->u16chars);
    NFree(self->u8chars );
}

NString *NStringCreateWithUBytes(NUTFType type, const void *begin, const void *end) {
    NString *self = NCreate(nisizeof(NString), NStringDeinit);

    int maxCharSize = 0;
    switch (type) {
    case NUTF32:
        self->u32chars = NDupU32FromUBytes(NUTF32, begin, end);
        break;
    case NUTF16:
        NCheckU16Bytes(begin, end, &maxCharSize);
        switch (maxCharSize) {
        case 4: self->u32chars = NDupU32FromUBytes(NUTF16, begin, end); break;
        case 2: self->u16chars = NDupU16FromUBytes(NUTF16, begin, end); break;
        default:;
        }
        break;
    case NUTF8:
        NCheckU8Bytes(begin, end, &maxCharSize);
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
    NString *self = NCreate(nisizeof(NString), NStringDeinit);

    int maxCharSize = 0;
    switch (type) {
    case NUTF32:
        self->u32chars = NDupU32FromUChars(NUTF32, chars);
        break;
    case NUTF16:
        NCheckU16Chars(chars, &maxCharSize);
        switch (maxCharSize) {
        case 4: self->u32chars = NDupU32FromUChars(NUTF16, chars); break;
        case 2: self->u16chars = NDupU16FromUChars(NUTF16, chars); break;
        default:;
        }
        break;
    case NUTF8:
        NCheckU8Chars(chars, &maxCharSize);
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
    return NCreate(nisizeof(NString), NStringDeinit);
}

NString *NStringCopy(NString *that) {
    if (that == NULL) {
        return NULL;
    }

    NString *self = NStringCreate();
    switch (NStringPreferredUTF(self)) {
    case NUTF32: self->u32chars = NDup(that->u32chars); break;
    case NUTF16: self->u16chars = NDup(that->u16chars); break;
    case NUTF8 : self->u8chars  = NDup(that->u8chars ); break;
    default:;
    }
    return self;
}

const char32_t *NStringU32Chars(NString *self) {
    if (!self) {
        return NULL;
    }

    if (NStringTryMake(self, NUTF32)) {
        return self->u32chars;
    } else {
        return U"\0";
    }
}

const char16_t *NStringU16Chars(NString *self) {
    if (!self) {
        return NULL;
    }

    if (NStringTryMake(self, NUTF16)) {
        return self->u16chars;
    } else {
        return u"\0";
    }
}

const char *NStringU8Chars(NString *self) {
    if (!self) {
        return NULL;
    }

    if (NStringTryMake(self, NUTF8)) {
        return self->u8chars;
    } else {
        return u8"\0";
    }
}

int NStringU32Size(NString *self) {
    if (!self) {
        return 0;
    }

    if (NStringTryMake(self, NUTF32)) {
        int size = NMemorySize(self->u32chars);
        return size - nisizeof(char32_t);
    } else {
        return 0;
    }
}

int NStringU16Size(NString *self) {
    if (!self) {
        return 0;
    }

    if (NStringTryMake(self, NUTF16)) {
        int size = NMemorySize(self->u16chars);
        return size - nisizeof(char16_t);
    } else {
        return 0;
    }
}

int NStringU8Size(NString *self) {
    if (!self) {
        return 0;
    }

    if (NStringTryMake(self, NUTF8)) {
        int size = NMemorySize(self->u8chars);
        return size - nisizeof(char);
    } else {
        return 0;
    }
}

int NStringLength(NString *self) {
    if (self) {
        int size = NStringU32Size(self);
        return size / nisizeof(char32_t);
    } else {
        return 0;
    }
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

void NStringAppend(NString *self, NString *that) {
}

int NStringCompare(NString *self, NString *that) {
    return 0;
}
