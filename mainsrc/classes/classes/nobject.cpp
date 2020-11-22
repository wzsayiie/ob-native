#include "nobject.h"
#include "basis.h"

NObject::NObject() {
    _count = 1;
}

NObject::~NObject() {
}

NObject *NObject::Retain() {
    nsynwith(this) {
        _count += 1;
    }
    return this;
}

void NObject::Release() {
    nsynwith(this) {
        if (--_count <= 0) {
            delete this;
        }
    }
}
