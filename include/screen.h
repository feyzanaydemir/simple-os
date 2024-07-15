#ifndef SCREEN_H
#define SCREEN_H


/*************************************************************
 * Macros
 ************************************************************/
#define WHITE_ON_BLACK 0x0f
#define RED_ON_WHITE 0xf4


/*************************************************************
 * Function Prototypes
 ************************************************************/
extern void kprint(const char *message);
extern void kprint_attr(const char *message, const char attr);
extern void kputchar(const char c);
extern void kputchar_attr(const char c, const char attr);
extern void clear_screen(void);

#endif
