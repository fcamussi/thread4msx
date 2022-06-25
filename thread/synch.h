/* Fernando Camussi
   fcamussi@gmail.com */


#ifndef  __SYNCH_H__
#define  __SYNCH_H__

#include "types.h"
#include "list.h"



typedef struct
{
    uint8_t value;
    list_t queue;
} sem_t;


void sem_create(sem_t *sem, uint8_t value);
void sem_destroy(sem_t *sem);
void sem_p(sem_t *sem);
void sem_v(sem_t *sem);
void delay(uint32_t vt);



#endif

