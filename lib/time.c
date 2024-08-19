/*************************************************************
 * Includes
 ************************************************************/
#include "../include/time.h"
#include "../include/ports.h"
#include "../include/stdlib.h"
#include "../include/string.h"
#include "../include/screen.h"


/*************************************************************
 * Private Function Definitions
 ************************************************************/
uint8_t _bcd_to_bin(uint8_t bcd) {
    return ((bcd / 16) * 10) + (bcd & 0x0F);
}

uint8_t _read_rtc(uint8_t reg) {
    port_byte_out(0x70, reg);
    return port_byte_in(0x71);
}


/*************************************************************
 * Public Function Definitions
 ************************************************************/
void read_rtc_date(rtc_time_t *rtc_date) {
    rtc_date->second = _bcd_to_bin(_read_rtc(0x00));
    rtc_date->minute = _bcd_to_bin(_read_rtc(0x02));
    rtc_date->hour = _bcd_to_bin(_read_rtc(0x04));
    rtc_date->day = _bcd_to_bin(_read_rtc(0x07));
    rtc_date->month = _bcd_to_bin(_read_rtc(0x08));
    rtc_date->year = _bcd_to_bin(_read_rtc(0x09));
}

void print_rtc_time(void) {
    rtc_time_t rtc_date;
    read_rtc_date(&rtc_date);

    char time_str[64] = "";
    char second_str[32] = "";
    char minute_str[32] = "";
    char hour_str[32] = "";
    char day_str[32] = "";
    char year_str[32] = "";
    
    itoa(rtc_date.second, second_str);
    itoa(rtc_date.minute, minute_str);
    itoa(rtc_date.hour, hour_str);
    itoa(rtc_date.day, day_str);
    itoa(rtc_date.year, year_str);

    strcat(time_str, months[(rtc_date.month) - 1]);
    strcat(time_str, " ");
    strcat(time_str, day_str);
    strcat(time_str, " 20");
    strcat(time_str, year_str);
    strcat(time_str, " ");
    if (rtc_date.hour < 10) {
        strcat(time_str, "0");
    }
    strcat(time_str, hour_str);
    strcat(time_str, ":");
    if (rtc_date.minute < 10) {
        strcat(time_str, "0");
    }
    strcat(time_str, minute_str);
    strcat(time_str, ":");
    if (rtc_date.second < 10) {
        strcat(time_str, "0");
    }
    strcat(time_str, second_str);

    kprint(time_str);
}
