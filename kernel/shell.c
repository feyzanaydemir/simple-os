/*************************************************************
 * Includes
 ************************************************************/
#include "../include/stdlib.h"
#include "../include/stdbool.h"
#include "../include/screen.h"
#include "../include/keyboard.h"
#include "../include/string.h"
#include "../include/pmm.h"
#include "../include/time.h"
#include "../include/timer.h"
#include "../include/shell.h"


/*************************************************************
 * Macros
 ************************************************************/
#define LINE_BUFFER_DEFAULT_SIZE 256
#define TOKEN_BUFFER_DEFAULT_SIZE 64
#define LINE_BUFFER_SIZE_LIMIT BLOCK_SIZE
#define TOKEN_BUFFER_SIZE_LIMIT BLOCK_SIZE
#define TOKEN_DELIM " \t\r\n\a"


/*************************************************************
 * Global Variables
 ************************************************************/
static char *line_buffer;
static char **token_buffer;
static uint32_t line_buffer_size = LINE_BUFFER_DEFAULT_SIZE; 
static uint32_t token_buffer_size = TOKEN_BUFFER_DEFAULT_SIZE;
static int shell_status = 0;
static bool_t is_caps = false;
static const char *game_list[] = {"space-invaders", "dinosaur", "pong"};


/*************************************************************
 * Private Function Prototypes
 ************************************************************/
static void _sh_play(char **args, int num_args);
static void _sh_ascii(char **args, int num_args);
static void _sh_date(char **args, int num_args);
static void _sh_echo(char **args);
static void _sh_sleep(char **args, int num_args);
static void _sh_clear(char **args, int num_args);
static void _sh_exit(void);
static void _sh_help(char **args, int num_args);
static char **_sh_split_line(char *line_buffer);
static void _sh_execute(char **args);


/*************************************************************
 * Public Function Definitions
 ************************************************************/
void sh_init(void) {
    line_buffer = malloc(LINE_BUFFER_DEFAULT_SIZE * sizeof(char));
    token_buffer = malloc(token_buffer_size * sizeof(char*));
    shell_status = 1;
    kprint("You are now in the shell environment. Type 'help' to list available commands.\n> ");
}

void sh_get_line(uint8_t scancode) {
    if (!shell_status) {
        return;
    }

    /* A key has just been released */
    if (scancode & 0x80) {
        /* TODO ? */
    }
        
    /* Pressed enter */
    if (scancode == 0x1c) {
        char **args = _sh_split_line(line_buffer);
        _sh_execute(args);

        /* Empty buffers */
        memset(token_buffer, 0, token_buffer_size);
        memset(line_buffer, 0, line_buffer_size);
        return;
    }

    char c = scancodes[(int)scancode];

    if (c == '\b') {
        int len = strlen(line_buffer);
        if (len != 0) {
            line_buffer[len] = 0;
            line_buffer[len - 1] = '\0';
            kputchar(c);
        }
    } else {
        /* Resize buffer if needed */
        if ((line_buffer_size * sizeof(char)) >= LINE_BUFFER_SIZE_LIMIT) {
            line_buffer_size += line_buffer_size;
            realloc(line_buffer, line_buffer_size * sizeof(char) + LINE_BUFFER_SIZE_LIMIT);
        }

        strccat(line_buffer, c);
        kputchar(c);
    }
}


/*************************************************************
 * Private Function Definitions
 ************************************************************/
char **_sh_split_line(char *line) {
    int token_buffer_size = TOKEN_BUFFER_DEFAULT_SIZE;
    char *token = strtok(line, TOKEN_DELIM);
    int position = 0;

    while (token) {
        token_buffer[position] = token;
        position++;

        if ((token_buffer_size * sizeof(char*)) >= LINE_BUFFER_SIZE_LIMIT) {
            token_buffer_size += token_buffer_size;
            token_buffer = realloc(token_buffer, token_buffer_size * sizeof(char*) + TOKEN_BUFFER_SIZE_LIMIT);
        }

        token = strtok(0, TOKEN_DELIM);
    }

    token_buffer[position] = 0;
    return token_buffer;
}

void _sh_execute(char **args) {
    int num_args = 0, i = 0;
    while (args[i++]) {
        num_args++;
    }

    if (num_args == 0) {
        kprint("\n> ");
        return;
    }

    if (strcmp(args[0], "play") == 0) {
        _sh_play(args, num_args);
    } else if (strcmp(args[0], "ascii") == 0) {
        _sh_ascii(args, num_args);
    } else if (strcmp(args[0], "date") == 0) {
        _sh_date(args, num_args);
    } else if (strcmp(args[0], "echo") == 0) {
        _sh_echo(args);
    } else if (strcmp(args[0], "sleep") == 0) {
        _sh_sleep(args, num_args);
    } else if (strcmp(args[0], "clear") == 0) {
        _sh_clear(args, num_args);
    } else if (strcmp(args[0], "exit") == 0) {
        _sh_exit();
    } else if (strcmp(args[0], "help") == 0) {
        _sh_help(args, num_args);
    } else {
        kprint("\n");
        kprint(args[0]);
        kprint(": command not found.\n> ");
    }
}

static void _sh_play(char **args, int num_args) {
    if (num_args < 2) {
        kprint("\nplay: missing operand\nTry 'play --help' for more information.\n> ");
    } else if (num_args > 2) {
        kprint("\nplay: extra operand '");
        kprint(args[2]);
        kprint("\n'Try 'play --help' for more information.\n> ");
    } else {
        if (strcmp(args[1], "--help") == 0) {
            kprint("\nUsage: play [OPTION]\n");
            kprint("Game list:\n");
            int i;
            for (i = 0; game_list[i]; i++) {
                kprint("    - ");
                kprint(game_list[i]);
                if (game_list[i + 1]) {
                    kprint("\n");
                }
            }

            kprint("\n> ");
            return;
        }

        int i;
        for (i = 0; game_list[i]; i++) {
            if (strcmp(args[1], game_list[i]) == 0) {
                kprint("\nStarting ");
                kprint(game_list[i]);
                kprint("\n> ");
                /* TODO: START GAME */
                return;
            }
        }
        
        kprint("\nGame not found.\nTry 'play --help' for more information.\n> ");
    }
}

static void _sh_ascii(char **args, int num_args) {
    kprint("\nNothing to show yet.\n> ");
}

static void _sh_date(char **args, int num_args) {
    if (num_args > 1) {
        if (strcmp(args[1], "--help") == 0) {
            kprint("\nUsage: date\n> ");
            return;
        }

        kprint("\ndate: extra operand '");
        kprint(args[1]);
        kprint("'\nTry 'date --help' for more information.\n> ");
    } else {
        kprint("\n");
        print_rtc_time();
        kprint("\n> ");
    }
}

static void _sh_echo(char **args) {
    kprint("\n");
    int i;
    for (i = 1; args[i]; i++) {
        if (i > 1) {
            kprint(" ");
        }

        kprint(args[i]);
    }

    kprint("\n> ");
}

static void _sh_sleep(char **args, int num_args) {
    if (num_args < 2) {
        kprint("\nsleep: missing operand\nTry 'sleep --help' for more information.\n> ");
    } else if (num_args > 2) {
        kprint("\nsleep: extra operand '");
        kprint(args[2]);
        kprint("'\nTry 'sleep --help' for more information.\n> ");
    } else {
        
        if (strcmp(args[1], "--help") == 0) {
            kprint("\nUsage: sleep [SECONDS]\n");
            kprint("SECONDS must be greater than or equal to 0.\n> ");
            return;
        }

        int seconds = atoi(args[1]);

        /* Negative or invalid input */
        if (((strcmp(args[1], "0") != 0) && seconds == 0) || (seconds < 0)) {
            kprint("\nsleep: invalid operand '");
            kprint(args[1]);
            kprint("'\nTry 'sleep --help' for more information.\n> ");
            return;
        }

        kprint("\n");
        sleep(seconds);
        kprint("\n> ");
    }
}

static void _sh_clear(char **args, int num_args) {
    if (num_args > 1) {
        if (strcmp(args[1], "--help") == 0) {
            kprint("\nUsage: clear\n> ");
            return;
        }

        kprint("\nclear: extra operand '");
        kprint(args[1]);
        kprint("'\nTry 'clear --help' for more information.\n> ");
    } else {
        clear_screen();
        kprint("> ");
    }
}

static void _sh_exit(void) {
    kprint("\nExiting shell, bye.\n");
    shell_status = 0;
}

static void _sh_help(char **args, int num_args) {
    kprint("\nCommand list:\n");
    kprint("    - play\n");
    kprint("    - ascii\n");
    kprint("    - date\n");
    kprint("    - echo\n");
    kprint("    - sleep\n");
    //kprint("    - tick\n");
    kprint("    - clear\n");
    kprint("    - exit\n");
    kprint("    - help\n> ");
};
