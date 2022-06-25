/* Fernando Camussi
   fcamussi@gmail.com */


#include "apps.h"
#include <stdio.h>
#include <string.h>
#include "msx1.h"
#include "msxdisk.h"
#include "synch.h"



int clear(int argc, char *argv[])
{
    argc, argv;

    printf("\x1b\x45");

    return 0;
}


int echo(int argc, char *argv[])
{
    int c;

    for (c = 1; c < argc; c++)
    {
        printf("%s ", argv[c]);
    }
    printf("\r\n");

    return 0;
}


int repeat(int argc, char *argv[])
{
    int c;

    if (argc < 2)
    {
        printf("Uso: %s texto\r\n", argv[0]);
        return -1;
    }

    while(1)
    {
        for (c = 1; c < argc; c++)
        {
            printf("%s ", argv[c]);
        }
        printf("\r\n");
    }
}


int loop(int argc, char *argv[])
{
    int c;
    argc;
    argv;

    c = 0;
    while(1)
    {
        c++;
    }
}


int chdrive(int argc, char *argv[])
{
    uint8_t lv, n;
    argc, argv;

    if (argc == 1)
    {
        n = get_default_drive();
        printf("%c:\r\n", n + 'A');
        return 0;
    }
    else if (argc != 2)
    {
        printf("Uso: %s [unidad]\r\n", argv[0]);
        return -1;
    }

    if (argv[1][0] > 'Z')
    {
        n = argv[1][0] - 'a';
    }
    else
    {
        n = argv[1][0] - 'A';
    }

    lv = get_login_vector();

    if (argv[1][1] == ':' && argv[1][2] == '\0' && (lv >> n) & 1U == 1)
    {
        set_default_drive(n);
    }
    else
    {
        printf("La unidad no existe\r\n");
        return -1;
    }

    return 0;
}


int ls(int argc, char *argv[])
{
    fcb_t fcb;
    search_t search;
    uint8_t s;
    argc, argv;

    memset(&fcb, 0, sizeof(fcb));
    strncpy(fcb.name, "???????????", 11);

    s = search_first(&fcb, &search);
    while (s == 0)
    {
        dir_print_name(&search.dir);
        s = search_next();
    }

    return 0;
}


int cat(int argc, char *argv[])
{
    fcb_t fcb;
    char buffer[100];
    char *p;
    uint32_t r;

    if (argc != 2)
    {
        printf("Uso: %s fichero\r\n", argv[0]);
        return -1;
    }

    memset(&fcb, 0, sizeof(fcb));
    fcb_set_name(&fcb, argv[1]);

    if (open(&fcb) != 0)
    {
        printf("Error al abrir el fichero: %s\r\n", argv[1]);
        return -1;
    }

    r = read(&fcb, 1, sizeof(buffer) - 1, buffer);
    while (r > 0)
    {
        buffer[r] = '\0';
        p = buffer;
        while (*p != '\0')
        {
            putchar(*p);
            p++;
        }
        r = read(&fcb, 1, sizeof(buffer) - 1, buffer);
    }

    close(&fcb);

    return 0;
}


int colors(int argc, char *argv[])
{
    uint8_t fg, bg;
    argc, argv;

    for (fg = 1; fg < 16; fg++)
    {
        for (bg = 1; bg < 16; bg++)
        {
             color(fg, bg, 0);
             delay(10);
        }
    }
    color(15, 4, 4);

    return 0;
}


int sounds(int argc, char *argv[])
{
    uint16_t c;
    argc, argv;

    sound(8,15);
    for (c = 0; c < 256; c+=5)
    {
        sound(0,c);
        delay(20);
    }
    sound(8,0);

    return 0;
}


int text2tape(int argc, char *argv[])
{
    fcb_t fcb;
    char buffer[256];
    char *p;
    uint16_t t;

    if (argc != 2)
    {
        printf("Uso: %s <fichero de texto>\r\n", argv[0]);
        return -1;
    }

    memset(&fcb, 0, sizeof(fcb));
    fcb_set_name(&fcb, argv[1]);

    if (open(&fcb) != 0)
    {
        printf("Error al abrir el fichero: %s\r\n", argv[1]);
        return -1;
    }

    if (tape_output_on(LONG_HEADER) != 0)
    {
        printf("Error al escribir en el datassette\r\n");
        return -1;
    }
    for (t = 0; t < 10; t++)
    {
        tape_output_data(0xEA); // assci file
    }
    for (t = 0; t < 6; t++)
    {
        tape_output_data(argv[1][t]);
    }
    tape_output_off();

    memset(buffer, 0x00, 256);
    while (read(&fcb, 1, 256, buffer) > 0)
    {
        p = buffer;
        tape_output_on(SHORT_HEADER);
        for (t = 0; t < 256; t++)
        {
            tape_output_data(*p);
            p++;
        }
        tape_output_off();
        memset(buffer, 0x00, 256);
    }

    tape_output_on(SHORT_HEADER);
    for (t = 0; t < 256; t++)
    {
        tape_output_data(0x1A);
    }
    tape_output_off();

    close(&fcb);

    return 0;
}


int reboot(int argc, char *argv[])
{
    argc, argv;

    reset();

    return 0;
}


static void dir_print_name(dir_t *dir)
{
    uint8_t c;

    for (c = 0; c < 8; c++) putchar(dir->name[c]);
    putchar('.');
    for ( ; c < 11; c++) putchar(dir->name[c]);
    putchar('\r');
    putchar('\n');
}

