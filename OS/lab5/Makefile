CC = gcc

all:
	$(CC) -fPIC -c lib1.c -o lib1.o -lm
	$(CC) -fPIC -c lib2.c -o lib2.o -lm
	$(CC) -shared -o libFirst.so lib1.o -lm
	$(CC) -shared -o libSecond.so lib2.o -lm
	sudo cp libFirst.so /usr/lib
	sudo cp libSecond.so /usr/lib
	$(CC) main2.c -lFirst -lm -o static.out -fsanitize=address
	$(CC) main.c -ldl -lm -o dynamic.out -fsanitize=address



clean:
	rm -f *.o *.so *.out