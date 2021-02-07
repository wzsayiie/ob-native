#pragma once

#if __cplusplus
extern "C" {
#endif

#if !__cplusplus
    //"charxx_t" are not builtin types for c.
    typedef unsigned short char16_t;
    typedef unsigned int   char32_t;
#endif

int cslen  (const char     *str);
int cslen16(const char16_t *str);
int cslen32(const char32_t *str);

int cscmp  (const char     *a, const char     *b);
int cscmp16(const char16_t *a, const char16_t *b);
int cscmp32(const char32_t *a, const char32_t *b);

char     *csdup  (const char     *str);
char32_t *csdup16(const char16_t *str);
char32_t *csdup32(const char32_t *str);

#if __cplusplus
}
#endif
