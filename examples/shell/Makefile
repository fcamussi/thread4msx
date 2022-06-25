CC = sdcc -mz80 -I../../backend -I../../thread
OPTS = --code-loc 0x8406 --data-loc 0 --no-std-crt0
LIBS = ../../backend/crt0msx.rel\
       ../../backend/heap.rel\
       ../../backend/msx1.rel\
       ../../backend/msxdisk.rel\
       ../../backend/interrupt.rel\
       ../../thread/list.rel\
       ../../thread/thread.rel\
       ../../thread/synch.rel


shell.msx: apps.rel shell.c shell.h
	$(CC) $(OPTS) $(LIBS) apps.rel shell.c
	../../bin/hex2bin shell.ihx
	../../bin/header shell.bin shell.msx 0x8400 0x8400

apps.rel: apps.c apps.h
	$(CC) -c apps.c

clean:
	rm -f *.asm *.lst *.rel *.sym *.map *.ihx *.lk *.noi shell.bin shell.msx

