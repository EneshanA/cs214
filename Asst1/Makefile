all: scannerCSVsorter.o mergesort.o
	gcc scannerCSVsorter.c mergesort.c -o scannerCSVsorter

scannerCSVsorter.o: scannerCSVsorter.c
	gcc -c scannerCSVsorter.c
mergesort.o: mergesort.c
	gcc -c mergesort.c

clean:
	rm ./scannerCSVsorter ./*.o
