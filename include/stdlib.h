#ifndef STDLIB_H
#define STDLIB_H


/*************************************************************
 * Includes
 ************************************************************/
#include <stddef.h>


/*************************************************************
 * Function Prototypes
 ************************************************************/
extern int atoi(const char *str);
extern void itoa(int n, char str[]);
extern void htoa(int n, char str[]);
extern void *malloc(size_t size);
extern void *realloc(void *head, size_t size);
extern void free(void *head);

#endif
