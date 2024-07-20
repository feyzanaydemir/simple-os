/*************************************************************
 * Includes
 ************************************************************/
#include "../include/screen.h"
#include "../include/string.h"
#include "../include/ports.h"


/*************************************************************
 * Macros
 ************************************************************/
#define VIDEO_ADDRESS 0xb8000
#define MAX_ROWS 25
#define MAX_COLS 80
/* Screen I/O ports */
#define VGA_PORT_CTRL 0x3d4
#define VGA_PORT_DATA 0x3d5


/*************************************************************
 * Private Function Prototypes
 ************************************************************/
void _kprint_at(const char *message, char attr, int row, int col);
int _kputchar_at(const char c, char attr, int row, int col);
void _set_cursor_offset(int offset);
int _get_cursor_offset(void);
int _get_offset(int row, int col);
int _get_offset_row(int offset);
int _get_offset_col(int offset);


/*************************************************************
 * Public Function Definitions
 ************************************************************/
/* Print a message at the cursor location */
void kprint(const char *message) {
    int offset = _get_cursor_offset();
    int row = _get_offset_row(offset);
    int col = _get_offset_col(offset);
    char attr = WHITE_ON_BLACK;

    _kprint_at(message, attr, row, col);
}

/* Print a message at the cursor location */
void kprint_attr(const char *message, const char attr) {
    int offset = _get_cursor_offset();
    int row = _get_offset_row(offset);
    int col = _get_offset_col(offset);

    _kprint_at(message, attr, row, col);
}

/* Print a char at the cursor location */
void kputchar(const char c) {
    int offset = _get_cursor_offset();
    int row = _get_offset_row(offset);
    int col = _get_offset_col(offset);
    char attr = WHITE_ON_BLACK;

    _kputchar_at(c, attr, row, col);
}

/* Print a char at the cursor location */
void kputchar_attr(const char c, const char attr) {
    int offset = _get_cursor_offset();
    int row = _get_offset_row(offset);
    int col = _get_offset_col(offset);

    _kputchar_at(c, attr, row, col);
}

void clear_screen(void) {
    int screen_size = MAX_COLS * MAX_ROWS;
    uint8_t *screen = (uint8_t*)VIDEO_ADDRESS;
    int i;
    
    for (i = 0; i < screen_size; i++) {
        screen[i * 2] = ' ';
        screen[i * 2 + 1] = WHITE_ON_BLACK;
    }

    _set_cursor_offset(_get_offset(0, 0));
}


/*************************************************************
 * Private Function Definitions
 ************************************************************/
/* Print a message at the specified location */
void _kprint_at(const char *message, char attr, int row, int col) {
    if (row < 0 || row >= MAX_ROWS || col < 0 || col >= MAX_COLS) {
        kprint_attr("kprint error: Coordinates are out of bounds.\n", 0x0f);
        return;
    }
    
    /* Set attribute if it's null */
    if (!attr) {
        attr = WHITE_ON_BLACK;
    }

    int offset = _get_offset(row, col);
    int i;
    for (i = 0; message[i] != 0; i++) {
        offset = _kputchar_at(message[i], attr, row, col);
        /* Compute row/col for next iteration */
        row = _get_offset_row(offset);
        col = _get_offset_col(offset);
    }
}

/* Print a char at the given location, return the next position */
int _kputchar_at(const char c, char attr, int row, int col) {
    uint8_t *video_memory = (uint8_t*) VIDEO_ADDRESS;
    int offset = _get_offset(row, col);

    /* Set attribute if it's null */
    if (!attr) {
        attr = WHITE_ON_BLACK;
    }

    if (c == '\n') {
        offset = _get_offset(row + 1, 0);
    } else if (c == '\b') {
        offset -= 2;
        video_memory[offset] = ' ';
        video_memory[offset + 1] = attr;
    } else {
        video_memory[offset] = c;
        video_memory[offset + 1] = attr;
        offset += 2;
    }

    /* Check if the offset is over screen size and scroll */
    if (offset >= MAX_ROWS * MAX_COLS * 2) {
        int i = 0;
        for (i = 1; i < MAX_ROWS; i++){
            int dest = _get_offset(i - 1, 0) + VIDEO_ADDRESS;
            int src = _get_offset(i, 0) + VIDEO_ADDRESS;

            memcpy((void*)dest, (void*)src, MAX_COLS * 2);
        }
            
        /* Blank last line */
        char *last_line = (char*)(_get_offset(MAX_ROWS - 1, 0) + (uint8_t*)VIDEO_ADDRESS);
        for (i = 0; i < MAX_COLS * 2; i++) {
            last_line[i] = 0;
        }

        offset -= 2 * MAX_COLS;
    }

    _set_cursor_offset(offset);
    return offset;
}

int _get_cursor_offset(void) {
    /*
     * Use the VGA ports to get the current cursor position
     * Ask for high byte of the cursor offset (data 14)
     * Ask for low byte (data 15)
     */
    port_byte_out(VGA_PORT_CTRL, 14);
    /* High byte: << 8 */
    int offset = port_byte_in(VGA_PORT_DATA) << 8;
    port_byte_out(VGA_PORT_CTRL, 15);
    offset += port_byte_in(VGA_PORT_DATA);

    /* Position * size of character cell */
    return offset * 2;
}

void _set_cursor_offset(int offset) {
    offset /= 2;
    port_byte_out(VGA_PORT_CTRL, 14);
    port_byte_out(VGA_PORT_DATA, (uint8_t)(offset >> 8));
    port_byte_out(VGA_PORT_CTRL, 15);
    port_byte_out(VGA_PORT_DATA, (uint8_t)(offset & 0xff));
}

int _get_offset(int row, int col) {
    return 2 * (row * MAX_COLS + col); 
}

int _get_offset_row(int offset) {
    return offset / (2 * MAX_COLS); 
}

int _get_offset_col(int offset) {
    return (offset - (_get_offset_row(offset) * 2 * MAX_COLS)) / 2;
}
