#include "nnumber.h"

nstruct(NNumber) {
    NValue value;
};

NNumber *NNumberCreateWithBool(bool value) {
    NNumber *self = NNumberCreate();
    self->value = NMakeBoolValue(value);
    return self;
}

NNumber *NNumberCreateWithInt(int64_t value) {
    NNumber *self = NNumberCreate();
    self->value = NMakeIntValue(value);
    return self;
}

NNumber *NNumberCreateWithUInt(uint64_t value) {
    NNumber *self = NNumberCreate();
    self->value = NMakeUIntValue(value);
    return self;
}

NNumber *NNumberCreateWithDbl(double value) {
    NNumber *self = NNumberCreate();
    self->value = NMakeDblValue(value);
    return self;
}

NNumber *NNumberCreate(void) {
    return NCreate(nisizeof(NNumber), NULL);
}

NNumber *NNumberCopy(NNumber *that) {
    if (that) {
        NNumber *copy = NNumberCreate();
        copy->value = that->value;
        return copy;
    }
    return NULL;
}

void NNumberSetBool(NNumber *self, bool value) {
    if (self) {
        self->value = NMakeBoolValue(value);
    }
}

void NNumberSetInt(NNumber *self, int64_t value) {
    if (self) {
        self->value = NMakeIntValue(value);
    }
}

void NNumberSetUInt(NNumber *self, uint64_t value) {
    if (self) {
        self->value = NMakeUIntValue(value);
    }
}

void NNumberSetDbl(NNumber *self, double value) {
    if (self) {
        self->value = NMakeDblValue(value);
    }
}

bool NNumberGetBool(NNumber *self) {
    if (self) {
        return NBoolValue(self->value);
    }
    return false;
}

int64_t NNumberGetInt64(NNumber *self) {
    if (self) {
        return NIntValue(self->value);
    }
    return 0;
}

int NNumberGetInt(NNumber *self) {
    return (int)NNumberGetInt64(self);
}

uint64_t NNumberGetUInt64(NNumber *self) {
    if (self) {
        return NUIntValue(self->value);
    }
    return false;
}

unsigned NNumberGetUInt(NNumber *self) {
    return (unsigned)NNumberGetUInt64(self);
}

double NNumberGetDouble(NNumber *self) {
    if (self) {
        return NDblValue(self->value);
    }
    return false;
}

float NNumberGetFloat(NNumber *self) {
    return (float)NNumberGetDouble(self);
}

int NNumberCompare(NNumber *self, NNumber *that) {
    double a = NNumberGetDouble(self);
    double b = NNumberGetDouble(that);
    
    if (a > b) {return  1;}
    if (a < b) {return -1;}
    else /***/ {return  0;}
}
