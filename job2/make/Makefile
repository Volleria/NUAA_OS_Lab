main :main.o math.o
	gcc main.o math.o -o main

math.o:math.c
	gcc -c math.c -o math.o

main.o:main.c
	gcc -c main.c -o main.o

clean:
	rm -rf *.o main
