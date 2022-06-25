AS = sdasz80
CC = sdcc -mz80


all: crt0msx.rel heap.rel msx1.rel msxdisk.rel interrupt.rel

crt0msx.rel: crt0msx.s
	$(AS) -o crt0msx.s

heap.rel: heap.s
	$(AS) -o heap.s

msx1.rel: msx1.c msx1.h
	$(CC) -c msx1.c

msxdisk.rel: msxdisk.c msxdisk.h
	$(CC) -c msxdisk.c

interrupt.rel: interrupt.s
	$(AS) -o interrupt.s

clean:
	rm -f *.asm *.lst *.rel *.sym

