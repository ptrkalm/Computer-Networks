CC = gcc
CFLAGS = -std=gnu99 -Wall -Wextra

server_www: main.o server.o sockwrap.o
	$(CC) $(CFLAGS) -o server_www main.o server.o sockwrap.o

main.o: main.c server.h
	$(CC) $(CFLAGS) -c main.c -o main.o

server.o: server.c server.h sockwrap.h
	$(CC) $(CFLAGS) -c server.c -o server.o

sockwrap.o: sockwrap.c sockwrap.h
	$(CC) $(CFLAGS) -c sockwrap.c -o sockwrap.o

clean:
	rm *.o

distclean:
	rm *.o transport
