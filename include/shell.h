#ifndef SHELL_H
#define SHELL_H


/*************************************************************
 * Includes
 ************************************************************/
#include <stdint.h>


/*************************************************************
 * Function Prototypes
 ************************************************************/
extern void sh_init(void);
extern void sh_get_line(uint8_t scancode);

#endif
