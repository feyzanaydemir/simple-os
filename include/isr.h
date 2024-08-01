#ifndef ISR_H
#define ISR_H

/*************************************************************
 * Includes
 ************************************************************/
#include "idt.h"


/*************************************************************
 * Macros
 ************************************************************/
#define NUM_ISR_ENTRIES 256
#define IRQ0 32
#define IRQ1 33
#define IRQ2 34
#define IRQ3 35
#define IRQ4 36
#define IRQ5 37
#define IRQ6 38
#define IRQ7 39
#define IRQ8 40
#define IRQ9 41
#define IRQ10 42
#define IRQ11 43
#define IRQ12 44
#define IRQ13 45
#define IRQ14 46
#define IRQ15 47


/*************************************************************
 * Typedefs and Structs
 ************************************************************/
typedef void (*isr_t)(registers_t*);


/*************************************************************
 * Variables
 ************************************************************/
static const char *exception_messages[] = {
    "Division By Zero", "Debug", "Non Maskable Interrupt",
    "Breakpoint", "Into Detected Overflow", "Out of Bounds",
    "Invalid Opcode", "No Coprocessor", "Double Fault",
    "Coprocessor Segment Overrun", "Bad TSS", "Segment Not Present",
    "Stack Fault", "General Protection Fault", "Page Fault",
    "Unknown Interrupt", "Coprocessor Fault", "Alignment Check",
    "Machine Check", "Reserved", "Reserved", "Reserved", "Reserved",
    "Reserved", "Reserved", "Reserved", "Reserved", "Reserved",
    "Reserved", "Reserved", "Reserved", "Reserved"
};


/*************************************************************
 * Function Prototypes
 ************************************************************/
extern void isr_init(void);
extern void isr_handler(registers_t *registers);
extern void isr_add_handler(uint8_t n, isr_t handler);

#endif
