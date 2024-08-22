/*************************************************************
 * Includes
 ************************************************************/
#include "../include/ports.h"
#include "../include/shell.h"
#include "../include/isr.h"


/*************************************************************
 * Macros
 ************************************************************/
#define SCANCODE_MAX 90


/*************************************************************
 * Private Function Definitions
 ************************************************************/
static void _keyboard_callback(registers_t *regs) {
    uint8_t scancode = port_byte_in(0x60);
    
    if (scancode > SCANCODE_MAX) {
        return;
    }

    sh_get_line(scancode);
}


/*************************************************************
 * Public Function Definitions
 ************************************************************/
void init_keyboard(void) {
    isr_add_handler(IRQ1, _keyboard_callback); 
}
