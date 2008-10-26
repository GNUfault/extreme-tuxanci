CFLAGS= -O2 -Wall -lpthread
CPP= gcc

all: 
	$(CPP) $(CFLAGS) -otuxanci-master main.c sockets.c resolve.c

clean:
	rm -f tuxanci-master
