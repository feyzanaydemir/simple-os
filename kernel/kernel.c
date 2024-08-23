/*************************************************************
 * Includes
 ************************************************************/
#include "../include/screen.h"
#include "../include/timer.h"
#include "../include/keyboard.h"
#include "../include/idt.h"
#include "../include/pmm.h"
#include "../include/vmm.h"
#include "../include/shell.h"


/*************************************************************
 * Public Function Definitions
 ************************************************************/
void kmain(void) {
    clear_screen();

    /* INTERRUPTS */
    idt_init();
    init_timer(100);
    init_keyboard();
    // asm ("int $17");
    // asm ("int $19");
    // asm ("int $18");
    
    /* MEMORY */
    init_memory();
    init_paging();
    clear_screen();

    kprint("                   ___  _               _         ___   ___    \n");
    kprint("                  / __|(_) _ __   _ __ | | ___   / _ \\ / __|   \n");
    kprint("                  \\__ \\| || '  \\ | '_ \\| |/ -_) | (_) |\\__ \\   \n");
    kprint("                  |___/|_||_|_|_|| .__/|_|\\___|  \\___/ |___/   \n");
    kprint("                                 |_|                           \n");
    kprint("Author: Feyzan Aydemir\n");
    kprint("Contact: feyzan.aydemir@gmail.com\n");
    kprint("Repository: github.com/feyzanaydemir/simple-os\n");
    kprint("                                                                            \n");
    kprint("This is a very basic 32-bit operating system written from scratch.\n");
    kprint("It includes essential features such as interrupt handling, memory \nmanagement and paging, drivers for screen and keyboard interactions and a \nsimple shell interface.\n");
    kprint("================================================================================\n");

    /* SHELL */    
    sh_init();
}
