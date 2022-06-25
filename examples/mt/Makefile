CC = sdcc -mz80 -I../../backend -I../../thread
OPTS = --code-loc 0x8406 --data-loc 0 --no-std-crt0
LIBS = ../../backend/crt0msx.rel\
       ../../backend/heap.rel\
       ../../backend/msx1.rel\
       ../../backend/interrupt.rel\
       ../../thread/list.rel\
       ../../thread/thread.rel\
       ../../thread/synch.rel


mt.msx: mt.c
	$(CC) $(OPTS) $(LIBS) mt.c
	../../bin/hex2bin mt.ihx
	../../bin/header mt.bin mt.msx 0x8400 0x8400

clean:
	rm -f *.asm *.lst *.rel *.sym *.map *.ihx *.lk *.noi mt.bin mt.msx

