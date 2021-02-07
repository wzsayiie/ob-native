#include "cstr.h"
#include "def.h"

static int next(const void **ptr, int sin) {
    int val = 0;
    switch (sin) {
        case 1 : val = **(char     **)ptr; break;
        case 2 : val = **(char16_t **)ptr; break;
        case 4 : val = **(char32_t **)ptr; break;
        default:;
    }

    *ptr = (char *)*ptr + sin;
    return val;
}

static int len(const void *str, int sin) {
    int cnt = 0;
    while (next(&str, sin)) {
        cnt += 1;
    }
    return cnt;
}

int cslen  (const char     *str) {return len(str, 1);}
int cslen16(const char16_t *str) {return len(str, 2);}
int cslen32(const char32_t *str) {return len(str, 4);}

static int cmp(const void *a, const void *b, int sin) {
    while (true) {
        int x = next(&a, sin);
        int y = next(&b, sin);

        if (x != y) {
            return x > y ? 1 : -1;
        }
        if (!x && !y) {
            break;
        }
    }
    return 0;
}

int cscmp  (const char     *a, const char     *b) {return cmp(a, b, 1);}
int cscmp16(const char16_t *a, const char16_t *b) {return cmp(a, b, 2);}
int cscmp32(const char32_t *a, const char32_t *b) {return cmp(a, b, 4);}

static void *dup(const void *str, int sin) {
    int   cnt = len(str, sin) + 1;
    void *dst = mcalloc(cnt, sin);
    
    mmove(dst, str, cnt * sin);

    return dst;
}

char     *csdup  (const char     *str) {return dup(str, 1);}
char32_t *csdup16(const char16_t *str) {return dup(str, 2);}
char32_t *csdup32(const char32_t *str) {return dup(str, 4);}
