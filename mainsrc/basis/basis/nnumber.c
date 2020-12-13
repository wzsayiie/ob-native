#include "nnumber.h"

nstruct(NNumber) {
    NValue value;
    NType  type ;
};

NNumber *NNumberCreateWithBool(bool value) {
    NNumber *self = NNumberCreate();
    self->value.intValue = value;
    self->type = NTypeInt64;
    return self;
}

NNumber *NNumberCreateWithInt(int64_t value) {
    NNumber *self = NNumberCreate();
    self->value.intValue = value;
    self->type = NTypeInt64;
    return self;
}

NNumber *NNumberCreateWithFlt(double value) {
    NNumber *self = NNumberCreate();
    self->value.fltValue = value;
    self->type = NTypeDouble;
    return self;
}

NNumber *NNumberCreate(void) {
    return NCreate(nisizeof(NNumber), NULL);
}

NNumber *NNumberCopy(NNumber *that) {
    if (that) {
        NNumber *copy = NNumberCreate();
        copy->value = that->value;
        copy->type = that->type;
        return copy;
    }
    return NULL;
}

void NNumberSetBool(NNumber *self, bool value) {
    if (self) {
        self->value.intValue = value;
        self->type = NTypeInt64;
    }
}

void NNumberSetInt(NNumber *self, int64_t value) {
    if (self) {
        self->value.intValue = value;
        self->type = NTypeInt64;
    }
}

void NNumberSetFlt(NNumber *self, double value) {
    if (self) {
        self->value.fltValue = value;
        self->type = NTypeDouble;
    }
}

bool NNumberGetBool(NNumber *self) {
    return (bool)NNumberGetInt64(self);
}

int64_t NNumberGetInt64(NNumber *self) {
    if (!self) {
        return 0;
    }
    
    switch (self->type) {
        case NTypeInt64 :return /* ... */self->value.intValue;
        case NTypeDouble:return (int64_t)self->value.fltValue;
        default:;
    }
    return 0;
}

int NNumberGetInt(NNumber *self) {
    return (int)NNumberGetInt64(self);
}

double NNumberGetDouble(NNumber *self) {
    if (!self) {
        return 0;
    }
    
    switch (self->type) {
        case NTypeInt64 :return (double)self->value.intValue;
        case NTypeDouble:return /* .. */self->value.fltValue;
        default:;
    }
    return 0;
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
