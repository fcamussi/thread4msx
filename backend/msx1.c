/* Fernando Camussi
   fcamussi@gmail.com */


#include "msx1.h"



char getchar() __naked
{
    __asm
    call    0x009F
    ld      l,a

    ret
    __endasm;
}


void putchar(char c) __naked
{
    c;

    __asm     
    ld      hl,#2
    add     hl,sp
    ld      a,(hl)
    call    0x00A2

    ret
    __endasm;
}


void reset() __naked
{
    __asm
    call    0x0000
    __endasm;
}


void screen(uint8_t n) __naked
{
    n;

    __asm
    push    ix
    ld      ix,#0
    add     ix,sp

    ld      a,4(ix)
    call    0x005F

    pop     ix
    ret
    __endasm;
}


void color(uint8_t fg, uint8_t bg, uint8_t border) __naked
{
    fg, bg, border;

    __asm
    push    ix
    ld      ix,#0
    add     ix,sp

    ld      a,4(ix)
    ld      (0xF3E9),a
    ld      a,5(ix)
    ld      (0xF3EA),a
    ld      a,6(ix)
    ld      (0xF3EB),a
    call    0x0062

    pop     ix
    ret
    __endasm;
}


void locate(uint8_t x, uint8_t y) __naked
{
    x, y;

    __asm
    push    ix
    ld      ix,#0
    add     ix,sp

    ld      h,4(ix)
    ld      l,5(ix)
    call    0x00C6

    pop     ix
    ret
    __endasm;
}


void sound(uint8_t reg, uint8_t value) __naked
{
    reg, value;

    __asm
    push    ix
    ld      ix,#0
    add     ix,sp

    ld      a,4(ix)
    ld      e,5(ix)
    call    0x0093

    pop     ix
    ret
    __endasm;
}


uint8_t tape_input_on() __naked
{
    __asm
    push    ix

    call    0x00E1
    jp      C,_tih_error
    ld      l,#0
    jp      _tih_end
_tih_error::
    ld      l,#1
    jp      _tih_end
_tih_end::
    pop     ix
    ret
    __endasm;
}


uint8_t tape_input_data(uint8_t *data) __naked
{
    data;

    __asm
    push    ix
    ld      ix,#0
    add     ix,sp

    push    ix
    call    0x00E4
    pop     ix
    jp      C,_tid_error
    ld      l,4(ix)
    ld      h,5(ix)
    ld      (hl),a
    ld      l,#0
    jp      _tid_end
_tid_error::
    ld      l,#1
    jp      _tid_end
_tid_end::
    pop     ix
    ret
    __endasm;
}


void tape_input_off() __naked
{
    __asm
    push    ix

    call    0x00E7

    pop     ix
    ret
    __endasm;
}


uint8_t tape_output_on(header_t header) __naked
{
    header;

    __asm
    push    ix
    ld      ix,#0
    add     ix,sp
    
    ld      a,4(ix)
    call    0x00EA
    jp      C,_toh_error
    ld      l,#0
    jp      _toh_end
_toh_error::
    ld      l,#1
    jp      _toh_end
_toh_end::
    pop     ix
    ret
    __endasm;
}


uint8_t tape_output_data(uint8_t data) __naked
{
    data;

    __asm
    push    ix
    ld      ix,#0
    add     ix,sp

    ld      a,4(ix)
    call    0x00ED
    jp      C,_tod_error
    ld      l,#0
    jp      _tod_end
_tod_error::
    ld      l,#1
    jp      _tod_end
_tod_end::
    pop     ix
    ret
    __endasm;
}


void tape_output_off() __naked
{
    __asm
    push    ix

    call    0x00F0

    pop     ix
    ret
    __endasm;
}

