/*************************************************************
 * Includes
 ************************************************************/
#include "../include/idt.h"
#include "../include/isr.h"
#include "../include/string.h"


/*************************************************************
 * Global Variables
 ************************************************************/
static idt_entry_t idt[NUM_IDT_ENTRIES];
static idtr_t idtr;


/*************************************************************
 * Public Function Definitions
 ************************************************************/
void idt_set_entry(uint8_t n, void* isr) {
    idt[n].isr_low = (uint32_t)isr & 0xFFFF;
    idt[n].kernel_cs = 0x08;
    idt[n].reserved = 0;
    idt[n].attributes = 0x8E; 
    idt[n].isr_high = (uint32_t)isr >> 16;
}

void idt_init(void) {
    idtr.base = (uintptr_t)&idt[0];
    idtr.limit = sizeof(idt) - 1;
    memset(&idt, 0, sizeof(idt));
    
    asm volatile("lidtl (%0)" : : "m" (idtr));
    isr_init();
}
