/* Fernando Camussi
   fcamussi@gmail.com */


#include "thread.h"
#include "interrupt.h"
#include <stdlib.h>
#include <string.h>
#include "list.h"



thread_t *current_thread; // el thread que se está ejecutando
thread_t *destroy_thread; // thread para ser destruido en el
                          // siguiente cambio de contexto
list_t scheduler; // lista circular con los threads
uint8_t switch_from_user; // indica si switch_thread es llamada por
                          // un usuario y no desde una interrupción
uint32_t vdp_ticks; // contador de interrupciones



/*
   Inicializa el entorno para usar threads
*/
void thread_init()
{
    static thread_t main_thread; // thread principal

    main_thread.sp = 0x00; // desconocido por el momento
    main_thread.stack = NULL; // se utiliza el stack principal
    main_thread.status = READY;
    list_create(&scheduler, 1); // crea una lista circular
    list_append(&scheduler, &main_thread);
    current_thread = &main_thread;
    destroy_thread = NULL;
    switch_from_user = 0;
    vdp_ticks = 0;

    // instala switch_thread para ser llamada en cada interrupción
    install_isr(switch_thread); 
}


/*
   Desinicializa el entorno para threads
   Es importante llamarla para que se restablezcan las interrupciones
*/
void thread_uninit()
{
    // desinstala switch_thread y ya no se llama en cada interrupción
    uninstall_isr();

    list_destroy(&scheduler);
}


/*
   Crea un thread
   stack_size es el tamaño de la pila para el thread
   fun es la función que se va a ejecutar
   arg son los argumentos que se le pasan a la función
*/
void thread_create(thread_t *thread, uint16_t stack_size,
                   void *(*fun)(void *), void *arg)
{
    // reserva el espacio para la pila
    thread->stack = (uint8_t *)malloc(stack_size);

    // se prepara la pila, inicialmente iret es la dirección de fun,
    // ret es la dirección a donde se va a retornar cuando finaliza
    // la función que se ejecuta y arg son los argumentos que se le
    // pasan a la función
    thread->stack[stack_size - 6] = (uint8_t)((uint16_t)fun);
    thread->stack[stack_size - 5] = (uint8_t)((uint16_t)fun >> 8);
    thread->stack[stack_size - 4] = (uint8_t)((uint16_t)ret);
    thread->stack[stack_size - 3] = (uint8_t)((uint16_t)ret >> 8);
    thread->stack[stack_size - 2] = (uint8_t)((uint16_t)arg);
    thread->stack[stack_size - 1] = (uint8_t)((uint16_t)arg >> 8);
    // el resto de la pila se inicializa con ceros
    memset(thread->stack, 0x00, stack_size - 6);

    // el stack pointer tiene que apuntar a la dirección donde
    // se encuentran los registros guardados en la pila para ser
    // restaurados en el cambio de contexto
    // en la primera ejecución los registros se restauran con ceros
    thread->sp = (uint16_t)(thread->stack + stack_size - 20 - 6);

    thread->status = READY;
    sem_create(&thread->join, 0);
    DI;
    list_append(&scheduler, thread);
    EI;
}


/*
   Espera hasta que finalice la ejecución de un thread y retorna el valor
   retornado por la función del thread
*/
void *thread_join(thread_t *thread)
{
    sem_p(&thread->join); // si thread no terminó se bloquea hasta que
                          // termine
    return thread->ret;
}


/*
   Duerme el thread actual
   Se asume que las interrupciones están deshabilitadas y se
   las mantiene deshabilitadas
*/
void thread_sleep()
{
    current_thread->status = BLOCKED;
    switch_from_user = 1;
    switch_thread();
    DI; // deshabilito las interrupciones ya que
        // el cambio de contexto las habilita
}


/*
   Cede la ejecución del thread actual a otro thread
*/
void thread_yield()
{
    DI;
    switch_from_user = 1;
    switch_thread();
    // EI;
    // no hace falta habilitar las interrupciones
    // ya que el cambio de contexto las habilita
}


/*
   Finaliza la ejecución de un thread
*/
void thread_finish(thread_t *thread)
{
    DI;
    // no podemos liberar la memoria acá, lo marcamos para
    // ser destruido en el siguiente cambio de contexto
    destroy_thread = thread;
    EI;
    thread_yield();
}


/*
   Código que se ejecuta cuando finaliza la ejecución de una función
   asociada a un thread
*/
static void ret() __naked
{
    // copia el registro HL (valor retornado) en current_thread->ret
    __asm
    ld      d,h
    ld      e,l
    ld      hl,(_current_thread)
    inc     hl
    inc     hl
    ld      (hl),e
    inc     hl
    ld      (hl),d
    __endasm;

    // incrementa el semáforo para que pueda ser tomado en thread_join
    // y finaliza
    sem_v(&current_thread->join);
    thread_finish(current_thread);
}


/*
   Hace el cambio de contexto entre el thread actual y el siguiente a
   ejecutarse
*/
static void switch_thread() __naked
{
    DI;
    SAVE_REGISTERS;
    READ_VDP_STATUS; // (ver interrupt.h)

    // guardo el registro SP en current_thread->sp
    __asm
    ld      hl,#0
    add     hl,sp
    ld      d,l
    ld      e,h
    ld      hl,(_current_thread)
    ld      (hl),d
    inc     hl
    ld      (hl),e
    __endasm;

    // se libera la memoria utilizada por el thread
    // marcado para ser destruido
    if (destroy_thread)
    {
        destroy_thread->status = FINISH;
        list_remove(&scheduler, destroy_thread);
        free(destroy_thread->stack);
        sem_destroy(&destroy_thread->join);
        destroy_thread = NULL;
    }

    // elijo el siguiente thread (que esté listo)
    do
    {
        scheduler.current = scheduler.current->next;
        current_thread = (thread_t *)scheduler.current->element;
    }
    while (current_thread->status != READY);

    // cargo current_thread->sp en el registro SP
    __asm
    ld        hl,(_current_thread)
    ld        e,(hl)
    inc        hl
    ld        d,(hl)
    ld      hl,#0
    add     hl,de
    ld      sp,hl
    __endasm;

    if (switch_from_user) goto from_user;
    vdp_ticks++;
    RESTORE_REGISTERS;
    EI;
    __asm
    reti // retorno desde una interrupción
    __endasm;
from_user:
    switch_from_user = 0; // reinicio la variable
    RESTORE_REGISTERS;
    EI;
    __asm
    ret // retorno desde una llamada de usuario
    __endasm;
}

