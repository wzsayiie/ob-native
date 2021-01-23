#include "nany.h"
#include "basis.h"

NAny::NAny() {
    mCount = 1;
}

NAny::~NAny() {
}

NAny *NAny::Retain() {
    nsynwith(this) {
        mCount += 1;
    }
    return this;
}

void NAny::Release() {
    nsynwith(this) {
        if (--mCount <= 0) {
            delete this;
        }
    }
}
