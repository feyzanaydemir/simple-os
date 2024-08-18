#ifndef TIME_H
#define TIME_H

/*************************************************************
 * Includes
 ************************************************************/
#include <stdint.h>


/*************************************************************
 * Typedefs and Structs
 ************************************************************/
typedef struct rtc_time {
    uint8_t second;
    uint8_t minute;
    uint8_t hour;
    uint8_t day;
    uint8_t month;
    uint8_t year;
} rtc_time_t;


/*************************************************************
 * Variables
 ************************************************************/
static const char *months[] = {
    "January",
    "February",
    "March",
    "April",
    "May",
    "June",
    "July",
    "August",
    "September",
    "October",
    "November",
    "December"
};


/*************************************************************
 * Function Prototypes
 ************************************************************/
extern rtc_time_t *get_rtc_time(void);
extern void print_rtc_time(void);

#endif
