#include "io.h"

#if ON_ANDROID

//only on android, the function needs to reimplemented.

#include <android/log.h>
#include <stdarg.h>

int printf(const char *format, ...) {
    va_list list;
    va_start(list, format);
    int n = __android_log_vprint(ANDROID_LOG_INFO, "zzz", format, list);
    va_end(list);
    return n;
}

#endif
