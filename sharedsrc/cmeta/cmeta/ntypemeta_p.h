#pragma once

#if !__cplusplus
    #error "c++ needed"
#endif

#include "nenviron.h"

struct StructAdder {
    StructAdder(const char *name, const char *super, int size);
};

#define AddStruct(nm)     static StructAdder _struct_##nm(#nm, NULL, nsizeof(nm))
#define AddClass( nm, sp) static StructAdder _struct_##nm(#nm, #sp , nsizeof(nm))

int StructsBegin();
int StructsEnd  ();
int FindStruct  (const char *name);

const char *StructName(int index);

int StructSuper(int index);
int StructSize (int index);
