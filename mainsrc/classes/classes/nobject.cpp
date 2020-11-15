#include "nobject.h"
#include "basis.h"

NObject::NObject() {
    _referenceCount = 1;
}

NObject::~NObject() {
}

NObject *NObject::Retain() {
    nsynwith(this) {
        _referenceCount += 1;
    }
    return this;
}

void NObject::Release() {
    nsynwith(this) {
        if (--_referenceCount <= 0) {
            delete this;
        }
    }
}
