/*************************************************************
 * Includes
 ************************************************************/
#include <stdint.h>
#include "../include/string.h"


/*************************************************************
 * Public Function Definitions
 ************************************************************/
void* memcpy(void *dest, const void *src, size_t n) {
    char *dest_c = dest;
    const char *src_c = src;

    while (n-- > 0) {
        *dest_c++ = *src_c++;
    }

	return dest;
}

void* memset(void *dest, int val, size_t n) {
    char *dest_c = dest;

    while (n-- > 0) {
        *dest_c++ = val;
    }
        
    return dest;
}

int strlen(char *s) {
    int i = 0;
    while (s[i] != '\0') {
        ++i;
    }

    return i;
}

void strccat(char *s, char c) {
    int len = strlen(s);
    s[len] = c;
    s[len + 1] = '\0';
}

void revstr(char *s) {
    int c, i, j;
    for (i = 0, j = strlen(s) - 1; i < j; i++, j--) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

char *strcat(char *dest, char *src) {
    char *d = dest;
    while (*d != '\0') {
        d++;
    }

    const char *s = src;
    while (*s != '\0') {
        *d = *s;
        d++;
        s++;
    }

    *d = '\0';

    return dest;
}

/* Returns < 0 if s1 < s2, 0 if s1 == s2, > 0 if s1 > s2 */
int strcmp(char s1[], char s2[]) {
    int i;
    for (i = 0; s1[i] == s2[i]; i++) {
        if (s1[i] == '\0') {
            return 0;
        }
    }

    return s1[i] - s2[i];
}

char *strchr(const char *str, int ch) {
    while (*str != '\0') {
        if (*str == ch) {
            return (char *)str;
        }

        str++;
    }
    
    return 0;
}

char *strtok(char *str, const char *delim) {
    static char *last = 0;
    if (str != 0) {
        last = str;
    } else if (last == 0) {
        return 0;
    }
    
    /* Skip leading delimiters */
    while (*last && strchr(delim, *last)) {
        last++;
    }
    
    if (*last == '\0') {
        return 0;
    }
    
    char *start = last;
    
    /* Find the end of the token */
    while (*last && !strchr(delim, *last)) {
        last++;
    }
    
    if (*last != '\0') {
        *last = '\0';
        last++;
    }
    
    return start;
}

char *strcpy(char *dest, const char *src) {
    char *start = dest;
    
    while (*src != '\0') {
        *dest = *src;
        dest++;
        src++;
    }
    
    *dest = '\0';
    
    return start;
}
