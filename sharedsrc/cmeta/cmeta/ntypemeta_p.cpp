#include "ntypemeta_p.h"
#include "basis.h"
#include "binlist.h"
#include "cerpool.h"

struct TypeEntry {
    const char *owningTag ;
    const char *owningName;
    int         owningSize;
    const char *superTag  ;
};

const TypeEntry PREFINED_TYPES[] = {
    {"00", "UNDEFINED", 0                  , NULL},
    {"01", "ANYSTRUCT", 0                  , NULL},
    {"02", "ANYPTR"   , nsizeof(void     *), NULL},

    {"03", "void"     , nsizeof(char      ), NULL},
    {"04", "bool"     , nsizeof(bool      ), NULL},
    {"05", "char"     , nsizeof(char      ), NULL},
    {"06", "char16_t" , nsizeof(char16_t  ), NULL},
    {"07", "char32_t" , nsizeof(char32_t  ), NULL},
    {"08", "int8_t"   , nsizeof(int8_t    ), NULL},
    {"09", "int16_t"  , nsizeof(int16_t   ), NULL},
    {"10", "int32_t"  , nsizeof(int32_t   ), NULL},
    {"11", "int64_t"  , nsizeof(int64_t   ), NULL},
    {"12", "uint8_t"  , nsizeof(uint8_t   ), NULL},
    {"13", "uint16_t" , nsizeof(uint16_t  ), NULL},
    {"14", "uint32_t" , nsizeof(uint32_t  ), NULL},
    {"15", "uint64_t" , nsizeof(uint64_t  ), NULL},
    {"16", "float"    , nsizeof(float     ), NULL},
    {"17", "double"   , nsizeof(double    ), NULL},

    {"18", "void*"    , nsizeof(void     *), NULL},
    {"19", "bool*"    , nsizeof(bool     *), NULL},
    {"20", "char*"    , nsizeof(char     *), NULL},
    {"21", "char16_t*", nsizeof(char16_t *), NULL},
    {"22", "char32_t*", nsizeof(char32_t *), NULL},
    {"23", "int8_t*"  , nsizeof(int8_t   *), NULL},
    {"24", "int16_t*" , nsizeof(int16_t  *), NULL},
    {"25", "int32_t*" , nsizeof(int32_t  *), NULL},
    {"26", "int64_t*" , nsizeof(int64_t  *), NULL},
    {"27", "uint8_t*" , nsizeof(uint8_t  *), NULL},
    {"28", "uint16_t*", nsizeof(uint16_t *), NULL},
    {"29", "uint32_t*", nsizeof(uint32_t *), NULL},
    {"30", "uint64_t*", nsizeof(uint64_t *), NULL},
    {"31", "float*"   , nsizeof(float    *), NULL},
    {"32", "double*"  , nsizeof(double   *), NULL},

    {"33", "NObject"  , nsizeof(NObject   ), NULL},
    {"34", "NString"  , nsizeof(NString   ), "33"},
    {"35", "NLambda"  , nsizeof(NLambda   ), "33"},

    {NULL}
};

static const char *TagFromName(const char *name) {
    static binlist list = {0};
    nsynonce() {
        blinit(&list, scmp);

        for (auto itm = PREFINED_TYPES; itm->owningTag; ++itm) {
            blinsert(&list, pw(itm->owningName), pw(itm->owningTag));
        }
    }

    if (!name) {
        return NULL;
    }

    auto tag = (const char *)blvalue(&list, pw(name)).asptr;
    if (tag) {
        return tag;
    }
    return name;
}

static TypeEntry *BorrowEntry(const TypeEntry *src) {
    static cerpool pool = {0};
    nsynonce() {
        cpinit(&pool, nsizeof(TypeEntry));
    }
    
    TypeEntry *dst = (TypeEntry *)cpborrow(&pool);
    if (src) {
        *dst = *src;
    }
    return dst;
}

static binlist *EntryList() {
    static binlist list = {0};
    nsynonce() {
        blinit(&list, scmp);

        for (auto itm = PREFINED_TYPES; itm->owningTag; ++itm) {
            auto entry = BorrowEntry(itm);
            blinsert(&list, pw(entry->owningTag), pw(entry));
        }
    }
    return &list;
}

TypeAdder::TypeAdder(const char *name, const char *super, int size) {
    const char *owningTag = TagFromName(name);
    const char *superTag = TagFromName(super);

    //is the entry exist?
    binlist *list = EntryList();
    auto entry = (TypeEntry *)blvalue(list, pw(owningTag)).asptr;
    //insert a new entry.
    if (!entry) {
        entry = (TypeEntry *)BorrowEntry(NULL);
        blinsert(list, pw(owningTag), pw(entry));
    }

    entry->owningTag  = owningTag;
    entry->owningName = name;
    entry->owningSize = size;
    entry->superTag   = superTag;
}

int StructsBegin() {
    return NTYPE_OBJECT;
}

int StructsEnd() {
    binlist *list = EntryList();
    return blcount(list);
}

static int FindTypeByTag(const char *tag) {
    binlist *list = EntryList();

    int index = blindex(list, pw(tag));
    if (index != -1) {
        return index;
    }
    return 0;
}

int FindType(const char *name) {
    const char *tag = TagFromName(name);
    return FindTypeByTag(tag);
}

static TypeEntry *EntryAt(int index) {
    binlist *list = EntryList();
    int count = blcount(list);

    if (1 <= index && index < count) {
        return (TypeEntry *)bloffset(list, index).asptr;
    }
    return NULL;
}

const char *TypeName(int index) {
    TypeEntry *entry = EntryAt(index);
    if (entry) {
        return entry->owningName;
    }
    return NULL;
}

int TypeSize(int index) {
    TypeEntry *entry = EntryAt(index);
    if (entry) {
        return entry->owningSize;
    }
    return 0;
}

int TypeSuper(int index) {
    TypeEntry *entry = EntryAt(index);
    if (!entry) {
        return 0;
    }

    if (entry->superTag) {
        return FindTypeByTag(entry->superTag);
    }
    return 0;
}
