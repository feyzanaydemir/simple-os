/*************************************************************
 * Includes
 ************************************************************/
#include "../include/pmm.h"
#include "../include/screen.h"


/*************************************************************
 * Macros
 ************************************************************/
#define MEMORY_POOL_START 0x10000
#define MEMORY_POOL_SIZE (32 * 1024 * 1024) /* 32MB*/
#define MAX_BLOCK_COUNT (MEMORY_POOL_SIZE / BLOCK_SIZE)


/*************************************************************
 * Variables
 ************************************************************/
void *memory_pool = (void*)MEMORY_POOL_START;
uint32_t free_block_count = MAX_BLOCK_COUNT;


/*************************************************************
 * Public Function Definitions
 ************************************************************/
void init_memory(void) {
    uint32_t *block_address = (uint32_t*)memory_pool;
    memory_block *block;

    /* Every block is free at the beginning */
    uint32_t i;
    for (i = 0; i < MAX_BLOCK_COUNT - 1; i++) {
        block = (memory_block*)block_address;
        block->link_size = 0;
        block->next = (memory_block*)(block_address + (BLOCK_SIZE / 4)); /* TODO */
        block_address += (BLOCK_SIZE / 4);
    }

    /* Last block points to Null */
    block = (memory_block*)block_address;
    block->next = 0;

    kprint("Initilized PMM.\n");
}

void *alloc_block(void) {
    void *block_address;
    memory_block* block;

    /* First free block */
    block_address = memory_pool;

    /* Update pool */
    block = (memory_block*)memory_pool;
    memory_pool = block->next;
    free_block_count -= 1;
    return block_address;
}

/* Recieves a virtual address, frees the block at it */
void free_block(void *block_address) {
    /* This becomes the top of the stack */
    memory_block *block = (memory_block*)block_address;
    block->link_size = 0;
    block->next = (memory_block*)memory_pool;

    /* Store physical address and unmap it */
    memory_pool = block;
    free_block_count += 1;
    return;
}
