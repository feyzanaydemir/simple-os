/*************************************************************
 * Includes
 ************************************************************/
#include "../include/stdlib.h"
#include "../include/stdbool.h"
#include "../include/screen.h"
#include "../include/string.h"
#include "../include/pmm.h"


/*************************************************************
 * Public Function Definitions
 ************************************************************/
static bool_t is_digit(char c) {
    return c >= '0' && c <= '9';
}

int atoi(const char *str) {
    int result = 0;
    int sign = 1;
    bool_t has_digits = false;

    /* Skip leading whitespaces */
    while (*str == ' ' || *str == '\t' || *str == '\n') {
        str++;
    }

    if (*str == '-') {
        sign = -1;
        str++;
    } else if (*str == '+') {
        str++;
    }

    while (is_digit(*str)) {
        result = result * 10 + (*str - '0');
        str++;
        has_digits = true;
    }

    /* Check for on-digit characters in the middle or no digits at all */
    if (!has_digits || (*str != '\0' && !is_digit(*str))) {
        /* Handle invalid input */
        return 0;
    }

    return sign * result;
}

void itoa(int n, char str[]) {
    int sign = n;
    if (sign < 0) {
        n = -n;
    }
    
    int i = 0;
    do {
        str[i++] = n % 10 + '0';
    } while ((n /= 10) > 0);

    if (sign < 0) {
        str[i++] = '-';
    }

    str[i] = '\0';
    revstr(str);
}

void htoa(int n, char str[]) {
    strccat(str, '0');
    strccat(str, 'x');
    char zeros = 0;

    int32_t tmp;
    int i;
    for (i = 28; i > 0; i -= 4) {
        tmp = (n >> i) & 0xF;
        if (tmp == 0 && zeros == 0) {
            continue;
        }

        zeros = 1;
        if (tmp > 0xA) {
            strccat(str, tmp - 0xA + 'a');
        } else {
            strccat(str, tmp + '0');
        }
    }

    tmp = n & 0xF;
    if (tmp >= 0xA) {
        strccat(str, tmp - 0xA + 'a');
    } else {
        strccat(str, tmp + '0');
    }
}

int align_alloc_size(size_t size) {
    if (size < BLOCK_SIZE) {
        return BLOCK_SIZE;
    }
    
    int quotient = size / BLOCK_SIZE;
    int remainder = size % BLOCK_SIZE;
    int closest = remainder > 0 ? (quotient + 1) * BLOCK_SIZE : quotient * BLOCK_SIZE;
    return closest;
}

void *malloc(size_t size) {
    size = align_alloc_size(size);
    uint32_t block_count = size / BLOCK_SIZE;

    if (block_count > free_block_count) {
        kprint_attr("Out of memory, panic! Halting the CPU.\n", 0xf4);
        asm volatile("hlt");
    }

    memory_block *head = (memory_block*)alloc_block();
    head->link_size = block_count;

    memory_block *current = head;
    uint32_t i;
    for (i = 0; i < (block_count - 1); i++) {
        current->next = (memory_block*)alloc_block();
        current = current->next;
    }

    return (void*)head;
}

void *realloc(void *head, size_t size) {
    size = align_alloc_size(size);
    uint32_t new_block_count = size / BLOCK_SIZE;
    uint32_t old_block_count = ((memory_block*)head)->link_size;
    uint32_t difference = new_block_count - old_block_count;

    if (difference == 0) {
        return head;
    }

    if (difference > free_block_count) {
        kprint_attr("Out of memory, panic! Halting the CPU.\n", 0xf4);
        asm volatile("hlt");
    }

    /* Find the current tail */
    memory_block *tail = (memory_block*)head;
    uint32_t i;
    for (i = 0; i < old_block_count - 1; i++) {
        tail = tail->next;
    }

    /* Allocate new blocks and link it to the tail */
    memory_block *addition_head = (memory_block*)malloc(difference * BLOCK_SIZE);
    tail->next = addition_head;

    /* Update the link size */
    ((memory_block*)head)->link_size = new_block_count;
    return head;
}

void free(void *head) {
    uint32_t block_count = ((memory_block*)head)->link_size;

    if (block_count == 1) {
        free_block(head);
    } else {
        void *tail = (head + (BLOCK_SIZE * (block_count - 1)));
        uint32_t i;
        for (i = 0; i < block_count; i++) {
            free_block(tail);
            tail -= BLOCK_SIZE;
        }
    }
}
