NumericalAnalyzer: NumericalAnalyzer.o libinfi.a
	gcc NumericalAnalyzer.o libinfi.a -o NumericalAnalyzer -lm

libinfi.a: infi.o
	ar rcs libinfi.a infi.o

infi.o: infi.c infi.h
	gcc -c infi.c

NumericalAnalyzer.o: NumericalAnalyzer.c infi.h
	gcc -c NumericalAnalyzer.c

clean:
	rm -f infi.o NumericalAnalyzer.o libinfi.a NumericalAnalyzer

.PHONY: clean