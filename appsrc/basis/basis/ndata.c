#include "ndata.h"

nclass(NData) {
    void *bytes;
};

static void _NDataClear(NData *data) {
    NFree(data->bytes);
}

NData *NDataCreateWithBytes(const void *bytes, int size) {
    NData *self = NDataCreate();
    if (bytes && size > 0) {
        self->bytes = NRealloc(self->bytes, size);
        NMoveMemory(self->bytes, bytes, size);
    }
    return self;
}

NData *NDataCreateWithSize(int size) {
    NData *self = NDataCreate();
    if (size > 0) {
        self->bytes = NRealloc(self->bytes, size);
    }
    return self;
}

NData *NDataCreate(void) {
    return NCreate(nisizeof(NData), _NDataClear);
}

NData *NDataCopy(NData *that) {
    if (that) {
        NData *data = NDataCreate();
        data->bytes = NDup(that->bytes);
        return data;
    }
    return NULL;
}

void NDataResize(NData *self, int size) {
    if (self) {
        self->bytes = NRealloc(self->bytes, size);
    }
}

void *NDataBytes(NData *self) {
    if (self) {
        return self->bytes;
    }
    return NULL;
}

int NDataSize(NData *self) {
    if (self) {
        return NMemorySize(self->bytes);
    }
    return 0;
}
