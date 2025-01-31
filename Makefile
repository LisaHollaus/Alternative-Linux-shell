CC = gcc
CCFLAGS = -Wall -I./include


all: imcsh

# Link all files into the final executable
imcsh: imcsh.o functions.o
	$(CC) $(CCFLAGS) -o imcsh imcsh.o functions.o

clean:
	- rm *.o
	- rm imcsh

