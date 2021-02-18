#include "cerpool.h"

static const int BOARD_CHIP_TOTAL = 64;

void cpinit(cerpool *pool, int sin) {
    mzero(pool, szof(*pool));
    pool->chipsz = szof(cpchip) + sin;
}

void cpdeinit(cerpool *pool) {
    cpboard *cursor = pool->first;
    while (cursor) {
        cpboard *board = cursor;
        cursor = cursor->next;
        mfree(board);
    }
    
    mzero(pool, szof(*pool));
}

static void allocboard(cerpool *pool) {
    int size = szof(cpboard) + BOARD_CHIP_TOTAL * pool->chipsz;
    cpboard *board = mcalloc(1, size);
    board->prev = NULL;
    board->next = pool->first;
    
    pool->total += BOARD_CHIP_TOTAL;
    if (pool->first) {
        pool->first->prev = board;
    }
    pool->first = board;
}

static void freeboard(cerpool *pool, cpboard *board) {
    pool->total -= BOARD_CHIP_TOTAL;
    if (pool->first == board) {
        pool->first = board->next;
        if (pool->first) {
            pool->first->prev = NULL;
        }
    } else {
        board->prev->next = board->next;
        if (board->next) {
            board->next->prev = board->prev;
        }
    }
    
    mfree(board);
}

static cpboard *vacboard(cerpool *pool) {
    cpboard *board = pool->first;
    for (; board; board = board->next) {
        if (board->cost < BOARD_CHIP_TOTAL) {
            return board;
        }
    }
    
    return NULL;
}

static cpchip *vacchip(cerpool *pool, cpboard *board) {
    cpchip *chip = (cpchip *)(board + 1);
    while (true) {
        if (chip->board == NULL) {
            return chip;
        }
        
        chip = pmove(chip, pool->chipsz);
    }
    
    return NULL;
}

void *cpborrow(cerpool *pool) {
    if (pool->cost == pool->total) {
        allocboard(pool);
    }
    
    cpboard *board = vacboard(pool);
    cpchip *chip = vacchip(pool, board);
    
    //NOTE: "chip->board" alse be used to mark if the chip is used.
    chip->board = board;
    board->cost += 1;
    pool->cost += 1;
    
    return (cpchip *)chip + 1;
}

void cpreturn(cerpool *pool, void *load) {
    cpchip  *chip  = (cpchip *)load - 1;
    cpboard *board = chip->board;
    
    chip->board = NULL;
    board->cost -= 1;
    pool->cost -= 1;
    
    if (board->cost == 0) {
        freeboard(pool, board);
    }
}
