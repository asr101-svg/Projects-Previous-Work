all: a3q1 a3q2

a3q1: a3q1_main.o a3q1_functions.o
	gcc a3q1_main.o a3q1_functions.o -o a3q1 -lm 

a3q2: a3q2_main.o a3q2_functions.o 
	gcc a3q2_main.o a3q2_functions.o -o a3q2 -lm 

a3q1_main.o: a3q1_main.c a3q1_header.h 
	gcc -Wall -std=c99 -c a3q1_main.c  a3q1_header.h -lm

a3q1_functions.o: a3q1_functions.c a3q1_header.h 
	gcc -Wall -std=c99 -c a3q1_functions.c a3q1_header.h -lm

a3q2_main.o: a3q2_main.c a3q2_header.h
	gcc -Wall -std=c99 -c a3q2_main.c -lm

a3q2_functions.o: a3q2_functions.c a3q2_header.h
	gcc -Wall -std=c99 -c a3q2_functions.c -lm
	
clean:
	rm *.o *.gch a3q1 a3q2
