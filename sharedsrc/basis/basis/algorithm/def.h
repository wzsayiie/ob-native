#pragma once

#include <stdbool.h> //bool.
#include <stddef.h>  //NULL.

#if __cplusplus
extern "C" {
#endif

//the size of type.
#define szof(val) ((int)sizeof(val))

//word:

typedef struct word {
    void *asptr;
    int   asint;
    float asflt;
} word;

word pw(void *p);
word iw(int   i);
word fw(float f);

//comparer:

typedef int (*cmpr)(word a, word b);

int pcmp(word a, word b); //compare void *.
int icmp(word a, word b); //compare int.
int fcmp(word a, word b); //compare float.
int scmp(word a, word b); //compare char *.

//memory management:

void *mcalloc (int   cnt, int sin);
void *mrealloc(void *ptr, int cnt, int sin);
void  mfree   (void *ptr);

void  mmove(void *dst, const void *src, int size);
void *pmove(void *ptr, int offset);
void  mzero(void *dst, int size);

//debug print.
void print(const char *fmt, ...);

#if __cplusplus
}
#endif
