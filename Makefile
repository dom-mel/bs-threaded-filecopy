
all:
	gcc -Wall -O2 -lpthread -lrt -o mycopy.out filecopy.c

clean:
	rm -f *.o
	rm -f *.out

