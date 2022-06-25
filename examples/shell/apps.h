/* Fernando Camussi
   fcamussi@gmail.com */


#ifndef  __APPS_H__
#define  __APPS_H__

#include "apps.h"
#include <stdio.h>
#include <string.h>
#include "msx1.h"
#include "msxdisk.h"



int clear(int argc, char *argv[]);
int echo(int argc, char *argv[]);
int repeat(int argc, char *argv[]);
int loop(int argc, char *argv[]);
int chdrive(int argc, char *argv[]);
int ls(int argc, char *argv[]);
int cat(int argc, char *argv[]);
int colors(int argc, char *argv[]);
int sounds(int argc, char *argv[]);
int reboot(int argc, char *argv[]);
int text2tape(int argc, char *argv[]);

static void dir_print_name(dir_t *dir);



#endif


