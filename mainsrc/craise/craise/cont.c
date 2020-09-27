#include "cont.h"
#include <string.h>

static threadlocal int8_t _rangers[RANGER_MAX_NUM][RANGER_MAX_SIZE];
static threadlocal int _current = 0;

cranger *localranger(void *ranger, size_t size) {
    if (ranger && size > 0) {
        _current += 1;
        _current %= RANGER_MAX_NUM;
        return memcpy(_rangers[_current], ranger, size);
    }
    return NULL;
}
