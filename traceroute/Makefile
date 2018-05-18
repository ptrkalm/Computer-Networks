CC = gcc
CFLAGS = -std=gnu99 -Wall -Wextra

traceroute: main.o traceroute.o receive.o send.o
	$(CC) $(CFLAGS) -o traceroute main.o traceroute.o receive.o send.o

main.o: main.c traceroute.h
	$(CC) $(CFLAGS) -c main.c -o main.o

traceroute.o: traceroute.c traceroute.h
	$(CC) $(CFLAGS) -c traceroute.c -o traceroute.o

receive.o: receive.c traceroute.h
	$(CC) $(CFLAGS) -c receive.c -o receive.o

send.o: send.c traceroute.h
	$(CC) $(CFLAGS) -c send.c -o send.o

clean:
	rm *.o

distclean:
	rm *.o traceroute
