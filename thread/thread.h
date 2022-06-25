/* Fernando Camussi
   fcamussi@gmail.com */


#ifndef  __THREAD_H__
#define  __THREAD_H__

#include "types.h"
#include "synch.h"



#define SAVE_REGISTERS\
    __asm\
    push    af\
    push    bc\
    push    de\
    push    hl\
    ex      af,af\
    exx\
    push    af\
    push    bc\
    push    de\
    push    hl\
    push    ix\
    push    iy\
    __endasm


#define RESTORE_REGISTERS\
    __asm\
    pop     iy\
    pop	    ix\
    pop	    hl\
    pop	    de\
    pop	    bc\
    pop	    af\
    exx\
    ex	    af,af\
    pop	    hl\
    pop	    de\
    pop	    bc\
    pop	    af\
    __endasm



/* Estados que puede tener un thread */
enum
{
    READY,
    BLOCKED,
    FINISH
};



struct thread_s
{
    // es indispensable que sp esté primero de todo
    // y luego esté ret, el orden del resto no importa
    uint16_t sp;
    void *ret;
    uint8_t *stack;
    uint8_t status;
    sem_t join; // semáforo para sincronizar con thread_join
};

typedef struct thread_s thread_t;



extern thread_t *current_thread;
extern uint32_t vdp_ticks;


void thread_init();
void thread_uninit();
void thread_create(thread_t *thread, uint16_t stack_size,
                   void *(*fun)(void *), void *arg);
void *thread_join(thread_t *thread);
void thread_sleep();
void thread_yield();
void thread_finish(thread_t *thread);

static void ret() __naked;
static void switch_thread() __naked;



#endif

