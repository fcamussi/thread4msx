/* Fernando Camussi
   fcamussi@gmail.com */


#ifndef  __SHELL_H__
#define  __SHELL_H__

#include "types.h"
#include "thread.h"

#define MAX_THREADS 5
#define MAX_ARGS 5
#define CMD_LINE_SIZE 50
#define THREAD_STACK_SIZE 600
#define PROGS { "clear",\
                "echo",\
                "repeat",\
                "loop",\
                "chdrive",\
                "ls",\
                "cat",\
                "colors",\
                "sounds",\
                "text2tape",\
                "reboot",\
                 NULL }
#define FUNCS { clear,\
                echo,\
                repeat,\
                loop,\
                chdrive,\
                ls,\
                cat,\
                colors,\
                sounds,\
                text2tape,\
                reboot }


typedef int (*func_t)(int argc, char *argv[]);


typedef struct
{
    int8_t prog_code;
    int argc;
    char *argv[MAX_ARGS + 1];
    thread_t thread;
    char cmd_line[CMD_LINE_SIZE];
} control_t;



static void kill();
static void ps();
static int8_t get_prog_code(char *prog);
static int8_t process_prog(char *cmd_line);
static void *exec(void *arg);
static void print_help();



#endif


