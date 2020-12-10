.PHONY: clean
all: server client
RM = rm -f

server: server.o
	gcc -o server server.o

client: client.o
	gcc -o client client.o

server.o: server.c
	gcc -Wall -g -c server.c

client.o: client.c
	gcc -Wall -g -c client.c

clean:
	$(RM) *.o