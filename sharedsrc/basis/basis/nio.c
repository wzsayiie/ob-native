#include "nio.h"
#include <stdarg.h>
#include <stdio.h>

#if N_OS_ANDROID
    #include <android/log.h>
    static void SysInfo (const char *s) {__android_log_write(ANDROID_LOG_INFO , "nnn", s);}
    static void SysError(const char *s) {__android_log_write(ANDROID_LOG_ERROR, "nnn", s);}
#else
    #include <stdio.h>
    static void SysInfo (const char *s) {fprintf(stdout, "%s\n", s);}
    static void SysError(const char *s) {fprintf(stderr, "%s\n", s);}
#endif

static void Print(const char *format, va_list args, void (*print)(const char *)) {
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
    Print(format, args, SysInfo);
    va_end(args);
}

void NPrintError(const char *format, ...) {
    va_list args;
    va_start(args, format);
    Print(format, args, SysError);
    va_end(args);
}

void NPutInfo(const char *string) {
    if (string && *string) {
        SysInfo(string);
    }
}

void NPutError(const char *string) {
    if (string && *string) {
        SysError(string);
    }
}

void __NError(const char *format, ...) {
    va_list args;
    va_start(args, format);
    Print(format, args, SysError);
    va_end(args);
}
