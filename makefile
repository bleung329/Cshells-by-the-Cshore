all: shell.c
	gcc -o Cshell shell.c
run: all
	./Cshell
	
