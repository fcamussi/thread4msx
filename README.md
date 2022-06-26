# thread4msx

Librería para programar threads en C para MSX.

En principio se trata de una prueba de concepto, simplemente quería implementar multitarea apropiativa (preemptive multitasking) en mi computadora MSX y probar algunos problemas típicos de sistemas operativos.

<img src="https://user-images.githubusercontent.com/75378876/175836223-fd2382e8-baa6-4956-8315-03707f2345bc.gif" alt="animacion-counters" width="600">

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
Crea un nuevo hilo y lo pone en ejecución. `thread` es una variable de tipo `thread_t`, `stack_size` es el tamaño de memoria que se reserva para la pila, `fun` es la función a ejecutarse y `arg` son los argumentos que se le pasan a la función `fun`. 

### thread_join
```
void *thread_join(thread_t *thread);
```
Espera hasta que finalice la ejecución de un thread y retorna el valor retornado por la función del thread.

### thread_sleep
```
void thread_sleep();
```
Bloquea el thread actual.

### thread_yield
```
void thread_yield();
```
Cede la ejecución del thread actual a otro thread.

### thread_finish
```
void thread_finish(thread_t *thread);
```
Finaliza la ejecución de un thread.

### sem_create
```
void sem_create(sem_t *sem, uint8_t value);
```
Crea un semáforo con el valor `value`. `sem` es una variable de tipo `sem_t`.

### sem_destroy
```
void sem_destroy(sem_t *sem);
```
Libera la memoria utilizada por un semáforo.

### sem_p
```
void sem_p(sem_t *sem);
```
Decrementa el valor de un semáforo. Si el nuevo valor es cero, el thread se bloquea y se agrega a la cola del semáforo.

### sem_v
```
void sem_v(sem_t *sem);
```
Si hay algún thread en la cola del semáforo lo remueve de la cola y lo prepara para que se siga ejecutando. Luego se incrementa el valor del semáforo.

### delay
```
void delay(uint32_t vt);
```
Demora vt interrupciones del VDP antes de continuar.


## Ejemplos

En el directorio `examples` hay 3 programas de prueba:

* ***mt***: Es el problema de los molinetes (turnstiles) donde varios procesos incrementan una variable compartida y se produce una condición de competencia (race condition), por lo que se pierden conteos. A menos que se serialice la zona crítica, es decir, el incremento de la variable compartida, utilizando semáforos mutex (semáforos con valor 1).
* ***shell***: Un pequeño shell que permite ejecutar programas hardcodeados en segundo plano.
* ***counters***: 10 threads contando en simultaneo y mostrando el resultado en pantalla.
