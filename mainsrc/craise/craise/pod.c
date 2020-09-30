#include "pod.h"

void memzero(void *ptr, size_t size) {
    if (ptr && size > 0) {
        memset(ptr, 0, size);
    }
}
