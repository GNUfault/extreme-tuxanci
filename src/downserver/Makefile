
#CC = /usr/local/gcc/bin/gcc
CC = gcc

CFLAGS = -g -O0 -std=c99 -D_GNU_SOURCE=1 -I../base -I../net
LIBS = 

FILES = ../base/list.o ../base/textFile.o ../base/buffer.o ../net/tcp.o

downServer: downServer.c $(FILES)
	$(CC) $(CFLAGS) $(LIBS) -o downServer downServer.c $(FILES)

clean:
	rm -rf *.o *.c~ *.h~ Makefile~
