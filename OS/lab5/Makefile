CC = gcc

all:
	$(CC) -fPIC -c lib1.c -o lib1.o -lm
	$(CC) -fPIC -c lib2.c -o lib2.o -lm
	$(CC) -shared -o libFirst.so lib1.o -lm
	$(CC) -shared -o libSecond.so lib1.o -lm
	sudo cp libFirst.so /usr/lib
	sudo cp libSecond.so /usr/lib
	$(CC) main2.c -lFirst -lm -o static.out
	$(CC) main.c -ldl -lm -o dynamic.out



clean:
	rm -f *.o *.so *.out