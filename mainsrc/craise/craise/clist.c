#include "clist.h"
#include "dearray.h"

defstruct(clist) {
    cobj    super;
    dearray array;
};

clist *newclist(void) {
    clist *list = newcobj(derives(clist));

    memzero(&list->array, sizeof(dearray));
    dearray_init(&list->array, sizeof(cval));

    return list;
}

void _delclist(clist *list) {
    dearray_deinit(&list->array);
    _delcobj(superof(list));
}
