# thread4msx

Librería para programar threads en C para MSX, programada en C y ensamblador de Z80.

En principio se trata de una prueba de concepto, simplemente quería implementar multitarea en mi computadora MSX y probar algunos problemas típicos de sistemas operativos.

## Funciones y primitivas

A continuación se detallan las funciones y primitivas implementadas, tanto para el manejo de threads como de semáforos para sincronización entre threads.

### thread_init
```
void thread_init();
```
Inicializa la librería.

### thread_uninit
```
void thread_uninit();
```
Desinicializa la librería.

### thread_create
```
void thread_create(thread_t *thread, uint16_t stack_size,
                   void *(*fun)(void *), void *arg);
```
Crea un nuevo hilo y lo pone en ejecución. `thread` es una variable de tipo `thread_t`, `stack_size` es el tamaño de memoria que se reserva para la pila, `fun` es la función a ejecutarse y `arg` es el argumento que recibe la función `fun`. 

### thread_join
```
void *thread_join(thread_t *thread);
```
Espera la finalización del hilo `thread`.

### thread_sleep
```
void thread_sleep();
```

void thread_yield();

void thread_finish(thread_t *thread);

void sem_create(sem_t *sem, uint8_t value);


void sem_destroy(sem_t *sem);
void sem_p(sem_t *sem);


void sem_v(sem_t *sem);

void delay(uint32_t vt);


ejemplos:
