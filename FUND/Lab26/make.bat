gcc -E queue.c -o queue.i
gcc -S queue.i -o queue.s
as queue.s -o queue.o
gcc -E main.c -o main.i
gcc -S main.i -o main.s
as main.s -o main.o
gcc main.o queue.o -o launchThis
