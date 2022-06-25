#include <stdio.h>
#include "thread.h"
#include "synch.h"
#include "msx1.h"
#include <stdlib.h>


#define N 10 // cantidad de threads



sem_t mutex; // semáforo
char tmp[10];


void *fun(void *arg)
{
    uint16_t c;

    for (c = 0; c <= 100; c++)
    {
        sem_p(&mutex);
        locate(15, 8+(uint16_t)arg);
        _ultoa(c, tmp, 10);
        puts((char*)tmp);
        sem_v(&mutex);
        thread_yield();
    }

    return arg;
}



void main()
{
    thread_t thr[N];
    uint8_t i;
    void *ret;

    thread_init();
    sem_create(&mutex, 1);

    // creamos los hilos
    for (i = 0; i < N; i++)
    {
        sem_p(&mutex);
        locate(1,8+i);
        printf("* counter %u:      *", i);
        sem_v(&mutex);
        thread_create(&thr[i], 400, fun, (void *)i);
    }
    // esperamos a que finalicen
    for (i = 0; i < N; i++)
    {
        ret = thread_join(&thr[i]);
        sem_p(&mutex);
        locate(1,8+i);
        printf("thread %u finalizado", (uint16_t)ret);
        sem_v(&mutex);
    }

    sem_destroy(&mutex);
    thread_uninit();
}


