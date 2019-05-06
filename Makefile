all:
	gcc -Wall -o crack crack.c -lcrypt

clean:
	rm crack

