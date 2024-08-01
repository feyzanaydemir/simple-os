#ifndef PMM_H
#define PMM_H


/*************************************************************
 * Includes
 ************************************************************/
#include <stddef.h>
#include <stdint.h>


/*************************************************************
 * Macros
 ************************************************************/
#define BLOCK_SIZE 0x1000 /* 4096, 4KB */


/*************************************************************
 * Typedefs and Structs
 ************************************************************/
typedef struct memory_block {
    struct memory_block* next;
    size_t link_size;
} memory_block;


/*************************************************************
 * Variables
 ************************************************************/
extern uint32_t free_block_count;


/*************************************************************
 * Function Prototypes
 ************************************************************/
extern void init_memory(void);
extern void *alloc_block(void);
extern void free_block(void *block);

#endif
