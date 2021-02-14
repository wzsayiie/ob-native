#include "cerpool.h"

static const int BOARD_CHIP_TOTAL = 64;

void cpinit(cerpool *pool, int sin) {
    mzero(pool, szof(*pool));
    pool->chipsz = szof(cpchip) + sin;
}

void cpdeinit(cerpool *pool) {
    cpboard *cursor = pool->head.next;
    while (cursor) {
        cpboard *board = cursor;
        cursor = cursor->next;
        mfree(board);
    }
    
    mzero(pool, szof(*pool));
}

static void allocboard(cerpool *pool) {
    cpboard *front = &pool->head;
    cpboard *back  = front->next;
    
    int size = szof(cpboard) + BOARD_CHIP_TOTAL * pool->chipsz;
    cpboard *board = mcalloc(1, size);
    board->prev = front;
    board->next = back;
    
    pool->total += BOARD_CHIP_TOTAL;
    front->next = board;
    if (back) {
        back->prev = board;
    }
}

static void freeboard(cerpool *pool, cpboard *board) {
    cpboard *front = board->prev;
    cpboard *back  = board->next;
    
    pool->total -= BOARD_CHIP_TOTAL;
    front->next = back;
    if (back) {
        back->prev = front;
    }
    
    mfree(board);
}

static cpboard *vacboard(cerpool *pool) {
    cpboard *board = pool->head.next;
    for (; board; board = board->next) {
        if (board->cost < BOARD_CHIP_TOTAL) {
            return board;
        }
    }
    
    return NULL;
}

static cpchip *vacchip(cerpool *pool, cpboard *board) {
    cpchip *chip = board->cuts;
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
    
    return chip->carry;
}

void cpreturn(cerpool *pool, void *carry) {
    cpchip  *chip  = (cpchip *)carry - 1;
    cpboard *board = chip->board;
    
    chip->board = NULL;
    board->cost -= 1;
    pool->cost -= 1;
    
    if (board->cost == 0) {
        freeboard(pool, board);
    }
}
