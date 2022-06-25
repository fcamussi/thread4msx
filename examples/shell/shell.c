/* Fernando Camussi
   fcamussi@gmail.com */


#include "shell.h"
#include <stdio.h>
#include <string.h>
#include "thread.h"
#include "apps.h"



control_t control[MAX_THREADS];
int16_t ret_value;
char **progs;
void **funcs;



void main()
{
    char const *p[] = PROGS;
    void *f[] = FUNCS;
    char cmd_line[CMD_LINE_SIZE];
    uint8_t readed, t;

    progs = p;
    funcs = f;

    for (t = 0; t < MAX_THREADS; t++)
    {
        control[t].thread.status = FINISH;
    }
    ret_value = 0;
    printf("Bienvenido, tipee help para ayuda\r\n");
    thread_init();

    while (1)
    {
        printf("%% ");
        gets(cmd_line); // overflow
        readed = strlen(cmd_line);
        cmd_line[readed] = '\0';
        if(readed > 0)
        {
            if (strcmp(cmd_line, "ret") == 0)
            {
                printf("%d\r\n", ret_value);
            }
            else if (strcmp(cmd_line, "ps") == 0)
            {
                ps();
            }
            else if (strcmp(cmd_line, "kill") == 0)
            {
                kill();
            }
            else if (strcmp(cmd_line, "exit") == 0)
            {
                thread_uninit();
                return;
            }
            else if (strcmp(cmd_line, "help") == 0)
            {
                print_help();
            }
            else
            {
                switch (process_prog(cmd_line))
                {
                    case -1:
                        printf("Maximo numero de threads ejecutandose\r\n");
                    break;
                    case -2:
                        printf("Comando invalido\r\n");
                    break;
                }
            }
        }
    }
}


static void ps()
{
    uint8_t t;

    printf("ID\tNAME\r\n");
    printf("-\tshell\r\n");
    for (t = 0; t < MAX_THREADS; t++)
    {
        if (control[t].thread.status != FINISH)
        {
            printf("%u\t%s\r\n", t, progs[control[t].prog_code]);
        }
    }
}


static void kill()
{
    int8_t t;

    printf("Proceso a finalizar? ");
    t = getchar() - '0';
    printf("\r\n");
    if (t >= 0 && t < MAX_THREADS &&
        control[t].thread.status != FINISH)
    {
        thread_finish(&control[t].thread);
        printf("Proceso %u finalizado\r\n", t);
    }
    else
    {
        printf("El proceso no existe\r\n", t);
    }
}


static int8_t get_prog_code(char *prog)
{
    uint8_t code = 0;

    while (progs[code])
    {
        if (strcmp(progs[code], prog) == 0)
        {
            return code;
        }
        code++;
    }

    return -1;
}


static int8_t process_prog(char *cmd_line)
{
    uint8_t t, s;
    int8_t bg;

    for (t = 0; t < MAX_THREADS; t++)
    {
        if (control[t].thread.status == FINISH)
        {
            break;
        }
    }
    if (t == MAX_THREADS)
    {
        return -1;
    }
    
    memcpy(control[t].cmd_line, cmd_line, CMD_LINE_SIZE);

    control[t].argv[0] = strtok(control[t].cmd_line, " ");
    if (strcmp(control[t].argv[0], "&") == 0)
    {
        bg = 1;
        s = 0;
    }
    else
    {
        bg = 0;
        s = 1;
    }
    while (s < MAX_ARGS)
    {
        control[t].argv[s] = strtok(NULL, " ");
        if (control[t].argv[s] == NULL)
        {
            break;
        }
        s++;
    }

    control[t].argc = s;
    control[t].argv[s] = NULL;

    control[t].prog_code = get_prog_code(control[t].argv[0]);
    if (control[t].prog_code < 0)
    {
        return -2;
    }
    else
    {
        if (bg)
        {
            thread_create(&control[t].thread, THREAD_STACK_SIZE, exec,
                          &control[t]);
        }
        else
        {
            thread_create(&control[t].thread, THREAD_STACK_SIZE, exec,
                          &control[t]);
            ret_value = (int8_t)thread_join(&control[t].thread);
        }
    }

    return 0;
}


static void *exec(void *arg)
{
    control_t *control = (control_t *)arg;
    int tmp;

    tmp = ((func_t)funcs[control->prog_code])(control->argc, control->argv);

    return (void *)tmp;
}


static void print_help()
{
    uint8_t i;

    printf("Comandos:\r\nret ps kill exit help\r\n"
           "prog: ejecuta prog en primer plano\r\n" 
           "& prog: ejecuta prog en segundo plano\r\n"
           "Programas compilados:\r\n");
    i = 0;
    while (progs[i])
    {
        printf("%s ", progs[i]);
        i++;
    }
    printf("\r\n");
}


