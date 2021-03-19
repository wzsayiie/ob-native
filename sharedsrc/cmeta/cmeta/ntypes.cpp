#include "ntypes.h"
#include "ntypemeta_p.h"

int NStructsBegin() {
    return StructsBegin();
}

int NStructsEnd() {
    return StructsEnd();
}

int NFindStruct(const char *name) {
    if (name && *name != '\0') {
        return FindStruct(name);
    }
    return 0;
}

const char *NStructName(int index) {
    return StructName(index);
}

int NStructSuper(int index) {
    return StructSuper(index);
}

int NStructSize(int index) {
    return StructSize(index);
}
