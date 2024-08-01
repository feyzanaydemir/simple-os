#ifndef IDT_H
#define IDT_H


/*************************************************************
 * Includes
 ************************************************************/
#include <stdint.h>


/*************************************************************
 * Macros
 ************************************************************/
#define NUM_IDT_ENTRIES 256


/*************************************************************
 * Typedefs and Structs
 ************************************************************/
/* How every interrupt gate (handler) is defined */
typedef struct {
    uint16_t    isr_low;    /* The lower 16 bits of the ISR's address */
	uint16_t    kernel_cs;  /* The GDT segment selector that the CPU will load into CS before calling the ISR */
	uint8_t     reserved;   /* Set to zero */
	uint8_t     attributes; /* Type and attributes; see the IDT page */
	uint16_t    isr_high;   /* The higher 16 bits of the ISR's address */
} __attribute__((packed)) idt_entry_t ;

typedef struct {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) idtr_t;

typedef struct {
   uint32_t ds;                                         /* Data segment selector */
   uint32_t edi, esi, ebp, useless, ebx, edx, ecx, eax; /* Pushed by pusha. */
   uint32_t int_no, err_code;                           /* Interrupt number and error code (if applicable) */
   uint32_t eip, cs, eflags, esp, ss;                   /* Pushed by the processor automatically */
} registers_t;


/*************************************************************
 * Function Prototypes
 ************************************************************/
extern void idt_init(void);
extern void idt_set_entry(uint8_t n, void* isr);

#endif
