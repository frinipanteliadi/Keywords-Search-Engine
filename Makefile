all: minisearch

minisearch: main.o functions.o
	gcc main.o functions.o -o minisearch -g -lm 

main.o: main.c
	gcc -c main.c -g

functions.o: functions.c
	gcc -c functions.c -g

clean:
	rm *.o minisearch 
