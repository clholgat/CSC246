CC = gcc
CFLAGS = -Wall -std=c99

fcfs: setup.c setup.h fcfs.c
	$(CC) $(CFLAGS) -g -o fcfs setup.c fcfs.c
	
sjf: setup.c setup.h sjf.c
	$(CC) $(CFLAGS) -g -o sjf setup.c sjf.c 
	
all: fcfs sjf
