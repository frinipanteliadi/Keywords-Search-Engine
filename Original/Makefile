all: minisearch

minisearch: test.o functions.o
	gcc test.o functions.o -o minisearch 

test.o: test.c
	gcc -c test.c 

functions.o: functions.c
	gcc -c functions.c 

clean:
	rm *.o minisearch 
