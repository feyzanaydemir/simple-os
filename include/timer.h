#ifndef TIMER_H
#define TIMER_H

/*************************************************************
 * Includes
 ************************************************************/
#include <stdint.h>


/*************************************************************
 * Function Prototypes
 ************************************************************/
extern void init_timer(uint32_t frequency);
extern void sleep(uint32_t seconds);

#endif
