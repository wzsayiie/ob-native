#include "nio.h"
#include <stdarg.h>
#include <stdio.h>

#if N_OS_ANDROID
    #include <android/log.h>
    static void _NSysInfo (const char *s) {__android_log_write(ANDROID_LOG_INFO , "nnn", s);}
    static void _NSysError(const char *s) {__android_log_write(ANDROID_LOG_ERROR, "nnn", s);}
#else
    #include <stdio.h>
    static void _NSysInfo (const char *s) {fprintf(stdout, "%s\n", s);}
    static void _NSysError(const char *s) {fprintf(stderr, "%s\n", s);}
#endif

static void _NPrint(const char *format, va_list args, void (*print)(const char *)) {
    if (!format || *format == '\0') {
        return;
    }

    char buffer[4096] = "\0";
    vsprintf(buffer, format, args);
    print(buffer);
}

void NPrintInfo(const char *format, ...) {
    va_list args;
    va_start(args, format);
    _NPrint(format, args, _NSysInfo);
    va_end(args);
}

void NPrintError(const char *format, ...) {
    va_list args;
    va_start(args, format);
    _NPrint(format, args, _NSysError);
    va_end(args);
}

void NPutInfo(const char *string) {
    if (string && *string) {
        _NSysInfo(string);
    }
}

void NPutError(const char *string) {
    if (string && *string) {
        _NSysError(string);
    }
}

void __NError(const char *format, ...) {
    va_list args;
    va_start(args, format);
    _NPrint(format, args, _NSysError);
    va_end(args);
}
