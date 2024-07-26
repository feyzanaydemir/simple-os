/*************************************************************
 * Includes
 ************************************************************/
#include "../include/isr.h"
#include "../include/screen.h"
#include "../include/stdlib.h"
#include "../include/ports.h"


/*************************************************************
 * Global Variables
 ************************************************************/
static isr_t interrupt_handlers[NUM_ISR_ENTRIES];
/* Defined in interrupts.asm */
extern void* isr_stub_table[];
extern void* irq_stub_table[];


/*************************************************************
 * Private Function Definitions
 ************************************************************/
void _remap_pic_irq(void) {
    port_byte_out(0x20, 0x11);
    port_byte_out(0xA0, 0x11);
    port_byte_out(0x21, 0x20);
    port_byte_out(0xA1, 0x28);
    port_byte_out(0x21, 0x04);
    port_byte_out(0xA1, 0x02);
    port_byte_out(0x21, 0x01);
    port_byte_out(0xA1, 0x01);
    port_byte_out(0x21, 0x0);
    port_byte_out(0xA1, 0x0);
}


/*************************************************************
 * Public Function Definitions
 ************************************************************/
void isr_init(void) {
    _remap_pic_irq();

    uint8_t n;
    for (n = 0; n < 48; n++) {
        if (n < 32) {
            idt_set_entry(n, isr_stub_table[n]);
        } else {
            idt_set_entry(n, irq_stub_table[n - 32]);
        }   
    }

    /* Enable interruptions */
    asm volatile("sti");
}

void isr_handler(registers_t *registers) {
    /* Handle exceptions */
    if (registers->int_no < 32) {
        char num[3];
        itoa(registers->int_no, num);
        kprint("Exception ");
        kprint(num);
        kprint(": ");
        kprint(exception_messages[registers->int_no]);
        kprint("\n");
        //asm volatile ("cli; hlt");
        /* TODO: Halt the cpu depending on the interrupt no */
    } else {
        /* Handle IRQs */
        if (registers->int_no >= 40) {
            /* Send reset signal to slave */
            port_byte_out(0xA0, 0x20);
        }

        port_byte_out(0x20, 0x20);

        if (registers->int_no < 48) {
            isr_t handler = interrupt_handlers[(registers->int_no) - 32];
            handler(registers);
        }
    }
}

void isr_add_handler(uint8_t n, isr_t handler) {
    interrupt_handlers[n - 32] = handler;
}
