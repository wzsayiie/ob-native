#include "nio.h"
#include <stdarg.h>
#include <stdio.h>

#if N_OS_ANDROID
    #include <android/log.h>
    #define _NPutS(string) __android_log_write(ANDROID_LOG_INFO, "nnn", string)
#else
    #include <stdio.h>
    #define _NPutS(string) puts(string)
#endif

#define SUMM_BUF_SIZE 4096
#define EACH_BUF_SIZE 1024

static nthreadlocal char sBufBegin[SUMM_BUF_SIZE];
#define sBufEnd (sBufBegin + SUMM_BUF_SIZE)

static nthreadlocal char *sPrint  = NULL;
static nthreadlocal char *sInsert = NULL;

void NPrintF(const char *format, ...) {
    if (sPrint == NULL) {
        sPrint  = sBufBegin;
        sInsert = sBufBegin;
    }
    
    //formatting arguments for string.
    va_list args;
    va_start(args, format);
    int count = vsnprintf(sInsert, sBufEnd - sInsert, format, args);
    va_end(args);
    
    //print line by line.
    char *begin = sPrint;
    char *endln = sInsert;
    for (; endln < sInsert + count; ++endln) {
        if (*endln != '\n') {
            continue;
        }
        
        *endln = '\0';
        NPutS(begin);
        begin = endln + 1;
    }
    
    sPrint  = begin;
    sInsert = endln;
    
    //if reserve buffer is insufficient, print cached string.
    if (sInsert + EACH_BUF_SIZE >= sBufEnd) {
        if (*sPrint) {
            NPutS(sPrint);
        }
        
        sPrint  = sBufBegin;
        sInsert = sBufBegin;
    }
}

void NPutS(const char *string) {
    if (string && *string) {
        _NPutS(string);
    }
}

void NFlush(void) {
    if (sPrint && *sPrint) {
        NPutS(sPrint);
    }
    
    sPrint  = sBufBegin;
    sInsert = sBufBegin;
}
