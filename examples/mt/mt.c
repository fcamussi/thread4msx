#include <stdio.h>
#include "thread.h"
#include "synch.h"


#define N 10 // cantidad de threads



uint16_t count; // variable compartida
sem_t mutex; // semáforo



void *fun(void *arg)
{
    uint16_t c;
    uint16_t tmp;

    for (c = 0; c < 5000; c++)
    {
        //sem_p(&mutex);
        tmp = count + 1; // no
        count = tmp;     // atomicidad
        //printf("thread %u corriendo\r\n", (uint16_t)arg);
        //sem_v(&mutex);
        //thread_yield();
    }

    return arg;
}



void main()
{
    thread_t thr[N];
    uint8_t i;
    void *ret;

    count = 0;
    thread_init();
    sem_create(&mutex, 1);

    // creamos los hilos
    for (i = 0; i < N; i++)
    {
        thread_create(&thr[i], 200, fun, (void *)i);
    }
    // esperamos a que finalicen
    for (i = 0; i < N; i++)
    {
        ret = thread_join(&thr[i]);
        printf("thread %u finalizado\r\n", (uint16_t)ret);
    }
    printf("count = %u\r\n", count);

    sem_destroy(&mutex);
    thread_uninit();
}


