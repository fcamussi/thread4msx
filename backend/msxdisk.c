/* Fernando Camussi
   fcamussi@gmail.com */


#include "msxdisk.h"
#include <string.h>



void fcb_set_name(fcb_t *fcb, char *name)
{
    uint8_t len, i;
    char *p;

    len = strlen(name);
    memset(fcb->name, ' ', 11);
    p = fcb->name;
    for (i = 0; i < len; i++)
    {
        if (name[i] == '.')
        {
            p += (8 - i);
            continue;
        }
        else
        {
            *p = name[i];
            p++;
        }
    }
}


uint8_t get_login_vector() __naked
{
    __asm
    push    ix

    ld      c,#OP_GETLOGVEC
    call    SC_DISK

    pop     ix
    ret
    __endasm;
}


void set_default_drive(uint8_t drive) __naked
{
    drive;

    __asm
    push    ix
    ld      ix,#0
    add     ix,sp

    ld      e,4(ix)
    ld      c,#OP_SETDEFDRV
    call    SC_DISK

    pop     ix
    ret
    __endasm;    
}


uint8_t get_default_drive() __naked
{
    __asm
    push    ix

    ld      c,#OP_GETDEFDRV
    call    SC_DISK

    pop     ix
    ret
    __endasm;
}


uint8_t create(fcb_t *fcb)
{
    return disk_op(fcb, OP_CREATE);
}


uint8_t rename(fcb_t *fcb)
{
    return disk_op(fcb, OP_RENAME);
}


uint8_t delete(fcb_t *fcb)
{
    return disk_op(fcb, OP_DELETE);
}


uint8_t open(fcb_t *fcb)
{
    return disk_op(fcb, OP_OPEN);
}


uint8_t close(fcb_t *fcb)
{
    return disk_op(fcb, OP_CLOSE);
}


void seek(fcb_t *fcb, uint32_t randomRecord)
{
    fcb->randomRecord = randomRecord;
}


uint16_t read(fcb_t *fcb, uint16_t recSize, uint16_t numRecs, char *buffer)
{
    fcb->recordSize = recSize;
    disk_op(buffer, OP_SETDMA);
    return random_read(fcb, numRecs);
}


uint8_t write(fcb_t *fcb, uint16_t recSize, uint16_t numRecs, char *buffer)
{
    fcb->recordSize = recSize;
    disk_op(buffer, OP_SETDMA);
    return random_write(fcb, numRecs);
}


uint8_t search_first(fcb_t *fcb, search_t *search)
{
    disk_op(search, OP_SETDMA);
    return disk_op(fcb, OP_SEARCH);
}


uint8_t search_next() __naked
{
    __asm
    push    ix

    ld      c,#OP_NSEARCH
    call    SC_DISK
    ld      l,a

    pop ix
    ret
    __endasm;
}


static uint8_t disk_op(void *ptr, uint8_t op) __naked
{
    ptr, op;

    __asm
    push    ix
    ld      ix,#0
    add     ix,sp

    ld      e,4(ix)
    ld      d,5(ix)
    ld      c,6(ix)
    call    SC_DISK
    ld      l,a

    pop     ix
    ret
    __endasm;
}


static uint16_t random_read(void *ptr, uint16_t numRecs) __naked
{
    ptr, numRecs;

    __asm
    push    ix
    ld      ix,#0
    add     ix,sp

    ld      e,4(ix)
    ld      d,5(ix)
    ld      c,#OP_RNDREAD
    ld      l,6(ix)
    ld      h,7(ix)
    call    SC_DISK

    pop     ix
    ret
    __endasm;
}


static uint8_t random_write(void *ptr, uint16_t numRecs) __naked
{
    ptr, numRecs;

    __asm
    push    ix
    ld      ix,#0
    add     ix,sp

    ld      e,4(ix)
    ld      d,5(ix)
    ld      c,#OP_RNDWRITE
    ld      l,6(ix)
    ld      h,7(ix)
    call    SC_DISK
    ld      l,a

    pop     ix
    ret
    __endasm;
}


