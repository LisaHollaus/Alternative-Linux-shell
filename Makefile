CC = gcc
CCFLAGS = -Wall

all: imcsh

imcsh: imcsh.o
	$(CC) $(CCFLAGS) -o imcsh imcsh.o -pthread

clean:
	- rm *.o
	- rm imcsh
