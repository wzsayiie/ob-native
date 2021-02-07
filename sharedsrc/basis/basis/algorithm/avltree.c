#include "avltree.h"

void atinit(avltree *tree, cmpr cmp) {
    cpinit(&tree->pool, szof(atnode));
    slinit(&tree->idat);

    tree->cmp  = cmp ;
    tree->root = NULL;
    tree->icur = 0;
}

void atdeinit(avltree *tree) {
    cpdeinit(&tree->pool);
    sldeinit(&tree->idat);

    tree->cmp  = NULL;
    tree->root = NULL;
    tree->icur = 0;
}

void atinsert(avltree *tree, word key, word value) {
}

word atremove(avltree *tree, word key) {
    return iw(0);
}

bool atexist(avltree *tree, word key) {
    return false;
}

int atcount(avltree *tree) {
    return 0;
}

void atpbgn(avltree *tree) {
}

void atrbgn(avltree *tree) {
}

bool atnext(avltree *tree) {
    return false;
}

word atread(avltree *tree) {
    return iw(0);
}
