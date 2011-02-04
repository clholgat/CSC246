CC = gcc
CFLAGS = 

server: server.c sock.c deck.c deck.h
	$(CC) $(CFLAGS) -g -o server server.c sock.c deck.c
	
client: client.c sock.c
	$(CC) $(CFLAGS) -g -o client client.c sock.c
	
all: client server
	
	
