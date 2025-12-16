main: main.c exec.c conf.c testfile.c
	gcc main.c exec.c conf.c testfile.c -o main.o -Wall -Werror
	./main.o

clean:
	rm ./main.o