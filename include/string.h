#ifndef STRING_H
#define STRING_H

/*************************************************************
 * Includes
 ************************************************************/
#include <stddef.h>


/*************************************************************
 * Function Prototypes
 ************************************************************/
extern void* memcpy(void *dest, const void *src, size_t len);
extern void* memset(void *dest, int val, size_t len);
extern void strccat(char *s, char c);
extern int strcmp(char *s1, char *s2);
extern char *strtok(char *str, const char *delim);
extern char *strcpy(char *dest, const char *src);
extern char *strcat(char *dest, char *src);
extern void revstr(char *s);
extern int strlen(char *s);

#endif
