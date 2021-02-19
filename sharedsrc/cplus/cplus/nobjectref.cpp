#include "nobjectref.h"

NObjectRef::NObjectRef() {
    mRef = NULL;
}

NObjectRef::NObjectRef(nullptr_t) {
    mRef = NULL;
}

NObjectRef::NObjectRef(const NObjectRef &that) {
    mRef = NRetain(that.mRef);
}

NObjectRef::NObjectRef(NObjectRef &&that) noexcept {
    mRef = that.mRef;
    that.mRef = NULL;
}

NObjectRef::~NObjectRef() {
    NRelease(mRef);
}

void NObjectRef::operator=(nullptr_t) {
    NRelease(mRef);
    mRef = NULL;
}

void NObjectRef::operator=(const NObjectRef &that) {
    NRelease(mRef);
    mRef = NRetain(that.mRef);
}

void NObjectRef::operator=(NObjectRef &&that) noexcept {
    NRelease(mRef);
    mRef = that.mRef;
    that.mRef = NULL;
}

void NObjectRef::setUnsafeRef(NRef ref) {
    NRelease(mRef);
    mRef = NRetain(ref);
}

NRef NObjectRef::unsafeRef() const {
    return mRef;
}

NObjectRef::operator NObject *() const {
    return (NObject *)mRef;
}
