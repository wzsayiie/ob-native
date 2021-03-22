#include "ntypes.h"
#include "ntypemeta_p.h"

int NStructsBegin() {
    return StructsBegin();
}

int NStructsEnd() {
    return StructsEnd();
}

int NFindType(const char *name) {
    if (name && *name != '\0') {
        return FindType(name);
    }
    return 0;
}

const char *NTypeName(int type) {
    return TypeName(type);
}

int NTypeSize(NType type) {
    return TypeSize(type);
}

int NTypeSuper(NType type) {
    return TypeSuper(type);
}
