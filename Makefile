
output: disk.o test.o
	gcc disk.o test.o -o output

disk.o: disk.c
	gcc -c disk.c

test.o: test.c
	gcc -c test.c

clean:
	rm *.o output disk0