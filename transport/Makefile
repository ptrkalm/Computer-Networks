CC = gcc
CFLAGS = -std=gnu99 -Wall -Wextra

transport: main.o transport.o receive.o send.o
	$(CC) $(CFLAGS) -o transport main.o transport.o receive.o send.o

main.o: main.c transport.h
	$(CC) $(CFLAGS) -c main.c -o main.o

transport.o: transport.c transport.h
	$(CC) $(CFLAGS) -c transport.c -o transport.o

receive.o: receive.c transport.h
	$(CC) $(CFLAGS) -c receive.c -o receive.o

send.o: send.c transport.h
	$(CC) $(CFLAGS) -c send.c -o send.o

clean:
	rm *.o

distclean:
	rm *.o transport
