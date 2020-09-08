#include "xcursor.h"
#include <string.h>

static threadlocal uint8_t _buf[CURSOR_MAX_NUM][CURSOR_MAX_SIZE];
static threadlocal int _idx = 0;

static void* storecursor(void* cursor, size_t size) {
    _idx = (_idx + 1) % CURSOR_MAX_NUM;
    return memcpy(_buf[_idx], cursor, size);
}

icursor *seticursor(void *c, size_t s) {return (icursor *)storecursor(c, s);}
scursor *setscursor(void *c, size_t s) {return (scursor *)storecursor(c, s);}

defstruct(intcursor) {
    icursor super;
    int step;
    int end;
};

static bool intnext(icursor *__cursor) {
    intcursor *cur = (intcursor *)__cursor;

    cur->super.val += cur->step;

    if ((cur->step > 0 && cur->super.val < cur->end)
     || (cur->step < 0 && cur->super.val > cur->end))
    {
        return true;
    }
    return false;
}

icursor *__intrange(int begin, int end, int step) {
    intcursor cursor;

    cursor.super.next = intnext;
    cursor.super.val = begin - step;
    cursor.step = step;
    cursor.end = end;

    return seticursor(&cursor, sizeof(cursor));
}
