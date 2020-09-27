#include "pod.h"
#include <string.h>

void mzero(void *mem, size_t size) {
    if (mem && size > 0) {
        memset(mem, 0, size);
    }
}
