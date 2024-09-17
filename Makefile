CC=gcc
CFLAGS=-I.

client: client.c requests.c helpers.c buffer.c client_commands.c parson.c
	$(CC) -o client client.c requests.c helpers.c buffer.c client_commands.c parson.c -Wall -g

run: client
	./client

clean:
	rm -f *.o client
