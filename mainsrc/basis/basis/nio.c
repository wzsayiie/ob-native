#include "nio.h"
#include "nenviron.h"
#include <stdarg.h>

#if N_OS_ANDROID
    #include <android/log.h>
#else
    #include <stdio.h>
#endif

void NPrintF(const char *format, ...) {
    va_list list;
    va_start(list, format);
    
    #if N_OS_ANDROID
        __android_log_vprint(ANDROID_LOG_INFO, "nnn", format, list);
    #else
        vprintf(format, list);
    #endif
    
    va_end(list);
}
