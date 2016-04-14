
all:	myshell

myshell:	myshell.o
	gcc myshell.o -o myshell

myshell.o:	myshell.c
	gcc -c myshell.c

run:	myshell
	@./myshell

clean:
	@rm myshell.o myshell
