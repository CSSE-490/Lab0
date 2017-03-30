CC=gcc
CFLAGS=-g -Wall -c
LDFLAGS=-g -Wall -o

all: client server

client: client.o globals.o
	$(CC) client.o globals.o $(LDFLAGS) client

client.o: client.c globals.h
	$(CC) client.c $(CFLAGS)

server: server.o globals.o
	$(CC) server.o globals.o $(LDFLAGS) server

server.o: server.c globals.h
	$(CC) server.c $(CFLAGS)

globals.o: globals.c globals.h
	$(CC) globals.c $(CFLAGS)

clean:
	rm -f *.o client server
