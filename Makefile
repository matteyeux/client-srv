CC = gcc 

all : client server

client : client.c
	gcc client.c -o client

server : server.c
	gcc server.c -o server

#dup : dup.c
#	gcc dup.c -o dup

clean :
	rm client server
