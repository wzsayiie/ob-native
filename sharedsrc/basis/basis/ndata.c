#include "ndata.h"

static void DataDestroy(NData *data) {
    NFreeMemory(data->bytes);
}

NData *NDataCreateWithBytes(const void *bytes, int size) {
    NData *data = NCreate(NData, DataDestroy);
    if (bytes && size > 0) {
        data->bytes = NAllocMemory(size);
        NMoveMemory(data->bytes, bytes, size);
    }
    return data;
}

NData *NDataCreateWithSize(int size) {
    NData *data = NCreate(NData, DataDestroy);
    if (size > 0) {
        data->bytes = NAllocMemory(size);
    }
    return data;
}

NData *NDataCreate(void) {
    return NCreate(NData, DataDestroy);
}

NData *NDataCopy(NData *that) {
    if (that) {
        NData *data = NDataCreate();
        data->bytes = NDupMemory(that->bytes);
        return data;
    }
    return NULL;
}

void *NDataResize(NData *data, int size) {
    if (data) {
        data->bytes = NReallocMemory(data->bytes, size);
        return data->bytes;
    }
    return NULL;
}

void *NDataBytes(NData *data) {
    if (data) {
        return data->bytes;
    }
    return NULL;
}

int NDataSize(NData *data) {
    if (data) {
        return NMemorySize(data->bytes);
    }
    return 0;
}
