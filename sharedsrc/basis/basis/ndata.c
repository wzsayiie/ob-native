#include "ndata.h"

void _NDataInitWithBytes(NData *data, const void *bytes, int size) {
    _NObjectInit(&data->Super);

    if (bytes && size > 0) {
        data->bytes = NAllocMemory(size);
        NMoveMemory(data->bytes, bytes, size);
    }
}

void _NDataInitWithSize(NData *data, int size) {
    _NObjectInit(&data->Super);

    if (size > 0) {
        data->bytes = NAllocMemory(size);
    }
}

void _NDataInit(NData *data) {
    _NObjectInit(&data->Super);
}

void _NDataDeinit(NData *data) {
    NFreeMemory(data->bytes);
    _NObjectDeinit(&data->Super);
}

NData *NDataCreateWithBytes(const void *bytes, int size) {
    NData *data = NAlloc(NData, _NDataDeinit);
    _NDataInitWithBytes(data, bytes, size);
    return data;
}

NData *NDataCreateWithSize(int size) {
    NData *data = NAlloc(NData, _NDataDeinit);
    _NDataInitWithSize(data, size);
    return data;
}

NData *NDataCreate(void) {
    NData *data = NAlloc(NData, _NDataDeinit);
    _NDataInit(data);
    return data;
}

NData *NDataCopy(NData *that) {
    if (that) {
        NData *data = NDataCreate();
        data->bytes = NDupMemory(that->bytes);
        return data;
    }
    return NULL;
}

void NDataResize(NData *data, int size) {
    if (data) {
        data->bytes = NReallocMemory(data->bytes, size);
    }
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
