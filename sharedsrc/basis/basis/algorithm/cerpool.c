#include "cerpool.h"

static const int EACH_ALLOC_NUM = 64;

void cpinit(cerpool *pool, int sin) {
    mzero(pool, szof(*pool));

    //NOTE: inner data size and user data size.
    pool->sin = szof(cpblock) + sin;
}

void cpdeinit(cerpool *pool) {
    mfree(pool->dat);
    mzero(pool, szof(*pool));
}

static cpblock *at(cerpool *pool, int idx) {
    return pmove(pool->dat, pool->sin * idx);
}

static cpblock *firstblank(cerpool *pool) {
    for (int i = 0; i < pool->sum; ++i) {
        cpblock *block = at(pool, i);
        if (!block->used) {
            return block;
        }
    }
    return NULL;
}

void *cpborrow(cerpool *pool) {
    cpblock *block = NULL;
    
    if (pool->use == pool->sum) {
        //allocate new blocks.
        pool->sum += EACH_ALLOC_NUM;
        pool->dat = mrealloc(pool->dat, pool->sum, pool->sin);
        
        block = at(pool, pool->use);
    } else {
        //find a unused block.
        block = firstblank(pool);
    }
    
    pool->use += 1;
    block->used = true;
    return block->load;
}

static int backblanks(cerpool *pool) {
    int count = 0;
    for (int i = pool->sum - 1; i >= 0; --i) {
        if (at(pool, i)->used) {
            break;
        }
        count += 1;
    }
    return count;
}

static void tryfree(cerpool *pool) {
    int num = pool->sum - pool->use;
    if (num < EACH_ALLOC_NUM) {
        return;
    }
    
    int blanks = backblanks(pool);
    blanks -= blanks % EACH_ALLOC_NUM;
    if (blanks > 0) {
        pool->sum -= blanks;
        pool->dat = mrealloc(pool->dat, pool->sum, pool->sin);
    }
}

void cpreturn(cerpool *pool, void *load) {
    cpblock *block = pmove(load, -szof(cpblock));
    //NOTE: clear the block to zero.
    mzero(block, pool->sin);
    pool->use -= 1;

    //try free excess blocks.
    tryfree(pool);
}
