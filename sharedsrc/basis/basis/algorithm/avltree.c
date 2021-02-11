#include "avltree.h"

void atinit(avltree *tree, cmpr cmp) {
    cpinit(&tree->pool, szof(atnode));
    slinit(&tree->idat);

    tree->cmp  = cmp  ;
    tree->root = NULL ;
    tree->ieff = false;
}

void atdeinit(avltree *tree) {
    cpdeinit(&tree->pool);
    sldeinit(&tree->idat);

    tree->cmp  = NULL ;
    tree->root = NULL ;
    tree->ieff = false;
}

atnode *atset(avltree *tree, word key, word value) {
    return NULL;
}

atnode *atget(avltree *tree, word key) {
    return NULL;
}

atnode atremove(avltree *tree, word key) {
    atnode node = {NULL};
    return node;
}

int atcount(avltree *tree) {
    return 0;
}

scalist *atsort(avltree *tree) {
    return NULL;
}
