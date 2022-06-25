/* Fernando Camussi
   fcamussi@gmail.com */


#ifndef  __LIST_H__
#define  __LIST_H__

#include "types.h"



struct node_s
{
    void *element;
    struct node_s *next;
};

typedef struct node_s node_t;


typedef struct
{
    uint8_t circular;
    node_t *first;
    node_t *last;
    node_t *current;
} list_t;



void list_create(list_t *list, uint8_t circular);
void list_destroy(list_t *list);
void list_append(list_t *list, void *element);
void list_remove_first(list_t *list);
void list_remove(list_t *list, void *element);



#endif

