/* Fernando Camussi
   fcamussi@gmail.com */


#ifndef  __MSX1_H__
#define  __MSX1_H__

#include "types.h"


typedef enum
{
    SHORT_HEADER,
    LONG_HEADER
} header_t;



void reset() __naked;
void screen(uint8_t n) __naked;
void color(uint8_t fg, uint8_t bg, uint8_t border) __naked;
void locate(uint8_t x, uint8_t y) __naked;
void sound(uint8_t reg, uint8_t value) __naked;
uint8_t tape_input_on() __naked;
uint8_t tape_input_data(uint8_t *data) __naked;
void tape_input_off() __naked;
uint8_t tape_output_on(header_t header) __naked;
uint8_t tape_output_data(uint8_t data) __naked;
void tape_output_off() __naked;



#endif

