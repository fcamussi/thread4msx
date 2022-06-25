
    .module crt0
    .globl  _main

    .area   _HEADER (ABS)
    .org    0x8400
init:
    ;; Initialize globals and jump to "main"
    call    gsinit
    jp      _main

    ; Program code and data (global vars) start here

    ; Place data after program code, and data init code after data
    .area   _CODE
    .area   _DATA

gsinit:
    .area   _GSINIT
    .area   _GSFINAL
    ret

    .area   _HEAP

