/* Fernando Camussi
   fcamussi@gmail.com */


#include "list.h"
#include <stdlib.h>



/*
   Crea una lista
   Si circular = 1 se comporta como una lista circular (round robin),
   si circular = 0 se comporta como una lista enlazada normal y puede
   usarse como una cola
*/
void list_create(list_t *list, uint8_t circular)
{
    list->circular = circular;
    list->first = NULL;
    list->last = NULL;
    list->current = NULL;
}


/*
   Libera la memoria usada por una lista
*/
void list_destroy(list_t *list)
{
    node_t *node, *tmp;

    node = list->first;
    while (node != list->last)
    {
        tmp = node;
        node = node->next;
        free(tmp);
    }
    free(node);
}


/*
   Agrega un elemento al final de una lista
*/
void list_append(list_t *list, void *element)
{
    node_t *node;

    node = (node_t *)malloc(sizeof(node_t));
    node->element = element;
    if (list->last)
    {
        list->last->next = node;
    }
    else
    {
        list->first = node;
        list->current = node;
    }
    list->last = node;
    node->next = (list->circular ? list->first : NULL);
}


/*
   Remueve el primer elemento de una lista
   Se asume que la lista no está vacía
*/
void list_remove_first(list_t *list)
{
    node_t *tmp;

    tmp = list->first;
    if (list->first == list->last) // hay un solo nodo
    {
        list->first = list->last = list->current = NULL;
    }
    else // hay más de un nodo
    {
        if (list->current == list->first)
        {
            list->current = list->first->next;
        }
        list->first = list->first->next;
        if (list->circular) list->last = list->first;
    }
    free(tmp);
}


/*
   Remueve un elemento de una lista
   Se asume que el elemento está en la lista
   Si el elemento aparece varias veces se remueve solo
   la primer ocurrencia
*/
void list_remove(list_t *list, void *element)
{
    node_t *node, *tmp;

    if (list->first == list->last) // hay un solo nodo
    {
        tmp = list->first;
        list->first = list->last = list->current = NULL;
    }
    else // hay más de un nodo
    {
        if (list->first->element == element) // es el primero
        {
            tmp = list->first;
            list->first = list->first->next;
            if (list->circular) list->last->next = list->first;
            if (tmp == list->current)
            {
                list->current = list->first;
            }
        }
        else
        {
            node = list->first;
            while (node->next->element != element)
            {
                node = node->next;
            }
            tmp = node->next;
            if (tmp == list->last)
            {
                list->last = node;
            }
            node->next = node->next->next;
            if (tmp == list->current)
            {
                list->current = node;
            }
        }
    }
    free(tmp);
}


