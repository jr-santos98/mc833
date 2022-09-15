all: cliente servidor

cliente: cliente.c
	gcc -Werror -Wall -g -lm -c cliente.c -o cliente

servidor: servidor.c
	gcc -Werror -Wall -g -lm -c servidor.c -o servidor

clean:
	rm -f cliente servidor