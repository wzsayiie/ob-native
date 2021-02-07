#pragma once

#include "cerpool.h"
#include "scalist.h"

#if __cplusplus
extern "C" {
#endif

typedef
struct atnode atnode;
struct atnode {
    word    key  ;
    word    value;
    atnode *left ;
    atnode *right;
};

//the balance tree with avl algorithm.
typedef struct avltree {
    cmpr    cmp ;
    cerpool pool;
    atnode *root;
    scalist idat;
    int     icur;
} avltree;

void atinit  (avltree *tree, cmpr cmp);
void atdeinit(avltree *tree);

void atinsert(avltree *tree, word key, word value);
word atremove(avltree *tree, word key);
bool atexist (avltree *tree, word key);
int  atcount (avltree *tree);

void atpbgn(avltree *tree);
void atrbgn(avltree *tree);
bool atnext(avltree *tree);
word atread(avltree *tree);

#if __cplusplus
}
#endif
