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

void *cpborrow(cerpool *pool) {
    if (pool->use == pool->sum) {
        //allocate new blocks.
        pool->sum += EACH_ALLOC_NUM;
        pool->dat = mrealloc(pool->dat, pool->sum, pool->sin);

        cpblock *block = at(pool, pool->use);
        pool->use += 1;
        block->used = true;
        return block;

    } else {
        //find a unused block.
        for (int i = 0; i < pool->sum; ++i) {
            cpblock *block = at(pool, i);
            if (block->used) {
                continue;
            }

            pool->use += 1;
            block->used = true;
            return block->load;
        }

        //the branch will not be arrived.
        return NULL;
    }
}

void cpreturn(cerpool *pool, void *load) {
    cpblock *block = pmove(load, -szof(cpblock));
    block->used = false;
    pool->use -= 1;

    //try free excess blocks.
    int vac = pool->sum - pool->use;
    if (vac < EACH_ALLOC_NUM) {
        return;
    }

    int serial = 0;
    for (int i = pool->sum - 1; i >= 0; --i) {
        if (at(pool, i)->used) {
            break;
        }
        serial += 1;
    }
    serial = serial - serial % EACH_ALLOC_NUM;
    if (serial > 0) {
        pool->sum -= serial;
        pool->dat = mrealloc(pool->dat, pool->sum, pool->sin);
    }
}
