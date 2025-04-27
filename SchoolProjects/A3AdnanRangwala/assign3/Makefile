UNAME := $(shell uname)
CC = gcc
CFLAGS = -Wall -std=c11 -g
LDFLAGS = -L$(BIN) -Wl,-rpath=$(BIN)
INC = include/
SRC = src/
BIN = bin/

main: main.o parser
	$(CC) $(CFLAGS) $(LDFLAGS) -o main main.o -lvcparser
	
main.o: $(SRC)main.c $(INC)LinkedListAPI.h $(INC)VCParser.h
	$(CC) $(CFLAGS) -I$(INC) -c $(SRC)main.c 

parser: VCParser.o LinkedListAPI.o
	$(CC) -shared -o $(BIN)libvcparser.so VCParser.o LinkedListAPI.o

VCParser.o: $(SRC)VCParser.c $(INC)VCParser.h 
	$(CC) -I$(INC) $(CFLAGS) -c -fpic $(SRC)VCParser.c

LinkedListAPI.o: $(SRC)LinkedListAPI.c $(INC)LinkedListAPI.h
	$(CC) -I$(INC) $(CFLAGS) -c -fpic $(SRC)LinkedListAPI.c


clean:
	rm -rf assign1 *.o *.so $(BIN)*.o $(BIN)*.so main
