CC = sdcc -mz80 -I../backend


all: list.rel synch.rel thread.rel

list.rel: list.c list.h
	$(CC) -c list.c

synch.rel: synch.c synch.h
	$(CC) -c synch.c

thread.rel: thread.c thread.h
	$(CC) -c thread.c

clean:
	rm -f *.asm *.lst *.rel *.sym

