#include "clist.h"
#include "dearray.h"
#include "cval.h"

defstruct(clist) {
    cobj    super;
    dearray array;
};

clist *newclist(void) {
    clist *list = newcobj(derives(clist));
    dearray_init(&list->array, sizeof(cval));
    return list;
}

void _delclist(clist *list) {
    dearray_deinit(&list->array);
    _delcobj(superof(list));
}
