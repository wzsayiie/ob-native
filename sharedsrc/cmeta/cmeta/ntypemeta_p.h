#pragma once

#if !__cplusplus
    #error "c++ needed"
#endif

#include "nenviron.h"

struct TypeAdder {
    TypeAdder(const char *name, const char *super, int size);
};

#define AddStruct(nm)     static TypeAdder _struct_##nm(#nm, NULL, nsizeof(nm))
#define AddClass( nm, sp) static TypeAdder _struct_##nm(#nm, #sp , nsizeof(nm))

int StructsBegin();
int StructsEnd  ();
int FindType    (const char *name);

const char *TypeName(int index);

int TypeSize (int index);
int TypeSuper(int index);
