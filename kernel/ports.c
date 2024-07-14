/*************************************************************
 * Includes
 ************************************************************/
#include "../include/ports.h"


/*************************************************************
 * Public Function Definitions
 ************************************************************/
/* Read a byte from the specified port */
uint8_t port_byte_in (uint16_t port) {
    uint8_t result;
    asm("in %%dx, %%al" : "=a" (result) : "d" (port));
    return result;
}

/* Write a byte out to the specified port */
void port_byte_out (uint16_t port, uint8_t data) {
    asm volatile("out %%al, %%dx" : : "a" (data), "d" (port));
}

/* Read a word from the specified port */
uint16_t port_word_in (uint16_t port) {
    uint16_t result;
    asm("in %%dx, %%ax" : "=a" (result) : "d" (port));
    return result;
}

/* Write a byte word to the specified port */
void port_word_out (uint16_t port, uint16_t data) {
    asm volatile("out %%ax, %%dx" : : "a" (data), "d" (port));
}
