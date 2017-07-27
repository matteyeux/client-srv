CC = gcc 
DEBUG ?=

ifeq ($(DEBUG), 1)
	DBG = -DDEBUG
endif

all : client server

client : client.c
	gcc client.c $(DBG) -o  $@

server : server.c
	gcc server.c sysnet/src/network.c -Isysnet $(DBG) -o  $@

clean :
	rm client server
