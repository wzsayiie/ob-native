#pragma once

#include "ustr.h"

defstruct(icursor) {
    bool (*next)(icursor *cursor);
    int val;
};

defstruct(scursor) {
    bool (*next)(icursor *cursor);
    ustr *val;
};

#define foreach(it, cursor)                           \
/**/    for (icursor *__c = cursor; __c->next(__c); ) \
/**/        for (bool __f = true; __f; )              \
/**/            for (it = __c->val; __f; __f = false)

#define CURSOR_MAX_NUM  6
#define CURSOR_MAX_SIZE (8 * sizeof(void *))

//store a cursor on current thread.
//NOTE: the size of "size" can't exceed CURSOR_MAX_SIZE.
icursor *seticursor(void *cursor, size_t size);
scursor *setscursor(void *cursor, size_t size);

//range-based traveral for integer.
//the last parameter "step" is optional, its default value is 1.
#define intrange(begin, end, ...) __intrange(begin, end, (1, ##__VA_ARGS__))
icursor *__intrange(int begin, int end, int step);
