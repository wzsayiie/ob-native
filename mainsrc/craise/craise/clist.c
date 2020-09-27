#include "clist.h"
#include "dearray.h"

defstruct(clist) {
    cobj    super;
    dearray array;
};

clist *newclist(void) {
    clist *list = newcobj(sizeof(clist), _delclist);

    mzero(&list->array, sizeof(dearray));
    list->array.isize = sizeof(cval);
    dearray_init(&list->array);

    return list;
}

void _delclist(clist *list) {
    dearray_clear(&list->array);
    _delcobj(&list->super);
}
