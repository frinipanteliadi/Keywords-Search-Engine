all: minisearch

minisearch: test.o functions.o
	gcc test.o functions.o -o minisearch -g -lm 

test.o: test.c
	gcc -c test.c -g

functions.o: functions.c
	gcc -c functions.c -g

clean:
	rm *.o minisearch 
