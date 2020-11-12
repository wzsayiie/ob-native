#pragma once

void *NAlloc  (int size);
void *NRealloc(void *ptr, int size);
void *NDup    (const void *ptr);
void  NFree   (void *ptr);

int  NMemorySize(const void *ptr);
void NZeroMemory(void *ptr, int size);
void NMoveMemory(void *dst, const void *src, int size);
