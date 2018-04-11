
output: disk.o test.o sfs.o
	gcc disk.o sfs.o test.o -o output

disk.o: disk.c
	gcc -c disk.c

sfs.o: sfs.c
	gcc -c sfs.c

test.o: test.c
	gcc -c test.c

clean:
	rm *.o output