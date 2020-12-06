#include "nio.h"
#include <stdarg.h>

#if N_OS_ANDROID
    #include <android/log.h>
    #define _NPutS(string) __android_log_write(ANDROID_LOG_INFO, "nnn", string)
#else
    #include <stdio.h>
    #define _NPutS(string) puts(string)
#endif

#define SUMM_BUF_SIZE 4096
#define EACH_BUF_SIZE 1024

static nthreadlocal char gBufBegin[SUMM_BUF_SIZE];
#define gBufEnd (gBufBegin + SUMM_BUF_SIZE)

static nthreadlocal char *gPrint  = NULL;
static nthreadlocal char *gInsert = NULL;

void NPrintF(const char *format, ...) {
    if (gPrint == NULL) {
        gPrint  = gBufBegin;
        gInsert = gBufBegin;
    }
    
    //formatting arguments for string.
    va_list args;
    va_start(args, format);
    int count = vsnprintf(gInsert, gBufEnd - gInsert, format, args);
    va_end(args);
    
    //print line by line.
    char *begin = gPrint;
    char *endln = gInsert;
    for (; endln < gInsert + count; ++endln) {
        if (*endln != '\n') {
            continue;
        }
        
        *endln = '\0';
        NPutS(begin);
        begin = endln + 1;
    }
    
    gPrint  = begin;
    gInsert = endln;
    
    //if reserve buffer is insufficient, print cached string.
    if (gInsert + EACH_BUF_SIZE >= gBufEnd) {
        if (*gPrint) {
            NPutS(gPrint);
        }
        
        gPrint  = gBufBegin;
        gInsert = gBufBegin;
    }
}

void NPutS(const char *string) {
    if (string && *string) {
        _NPutS(string);
    }
}

void NFlush(void) {
    if (gPrint && *gPrint) {
        NPutS(gPrint);
    }
    
    gPrint  = gBufBegin;
    gInsert = gBufBegin;
}
