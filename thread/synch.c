/* Fernando Camussi
   fcamussi@gmail.com */


#include "synch.h"
#include "interrupt.h"
#include "thread.h"



/*
   Crea un semáforo
*/
void sem_create(sem_t *sem, uint8_t value)
{
    list_create(&sem->queue, 0); // crea una cola
    sem->value = value;
}


/*
   Libera la memoria usada por un semáforo
*/
void sem_destroy(sem_t *sem)
{
    list_destroy(&sem->queue);
}


/*
   Espera un semáforo
*/
void sem_p(sem_t *sem)
{
    DI;
    while (sem->value == 0)
    {
        // pone en cola el thread
        list_append(&sem->queue, current_thread);
        // duerme el thread y espera a ser despertado
        thread_sleep();
    }
    sem->value--; // decrementa el valor del semáforo
    EI;
}


/*
   Incrementa el valor de un semáforo
*/
void sem_v(sem_t *sem)
{
    DI;
    if (sem->queue.first) // si hay algún thread en la cola...
    {
        // lo remueve de la cola y lo prepara para que se
        // siga ejecutando
        ((thread_t *)sem->queue.first->element)->status = READY;
        list_remove_first(&sem->queue);
    }
    sem->value++; // incrementa el valor del semáforo
    EI;
}


/*
    Demora vt interrupciones antes de continuar
*/
void delay(uint32_t vt)
{
    uint32_t vt1, vt2;

    DI;
    vt1 = vdp_ticks;
    EI;
    vt1 += vt;

wait:
    DI;
    vt2 = vdp_ticks;
    EI;
    if (vt2 < vt1)
    {
        thread_yield();
        goto wait;
    }
}


