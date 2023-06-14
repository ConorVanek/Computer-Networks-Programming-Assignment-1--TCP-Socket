CC = gcc
CFLAGS = -Wall -Wextra

all: server client

server: server.c
	$(CC) $(CFLAGS) -o ctserver server.c

client: client.c
	$(CC) $(CFLAGS) -o client client.c

clean:
	rm -f server client