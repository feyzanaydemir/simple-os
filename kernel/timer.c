/*************************************************************
 * Includes
 ************************************************************/
#include "../include/timer.h"
#include "../include/isr.h"
#include "../include/ports.h"
//#include "../include/screen.h"


/*************************************************************
 * Global Variables
 ************************************************************/
static volatile uint32_t timer_ticks = 0;


/*************************************************************
 * Private Function Definitions
 ************************************************************/
static void _timer_callback(registers_t* regs) {
    timer_ticks++;
    // Send end-of-interrupt (EOI) signal to the PIC
    //port_byte_out(0x20, 0x20);
}


/*************************************************************
 * Public Function Definitions
 ************************************************************/
void sleep(uint32_t seconds) {
    asm volatile ("cli");
    uint32_t end_tick = timer_ticks + (seconds * (1000 / 10));
    asm volatile ("sti");

    // Wait until the tick count reaches the end tick
    while (timer_ticks < end_tick) {
        asm volatile ("hlt");
    }
}

void init_timer(uint32_t frequency) {
    isr_add_handler(IRQ0, _timer_callback);

    /* Get the PIT value: hardware clock at 1193180 Hz */
    uint32_t divisor = 1193180 / frequency;
    uint8_t low  = (uint8_t)(divisor & 0xFF);
    uint8_t high = (uint8_t)( (divisor >> 8) & 0xFF);

    /* Send the command */
    port_byte_out(0x43, 0x36);
    port_byte_out(0x40, low);
    port_byte_out(0x40, high);
}
