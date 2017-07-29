CC = gcc 
SRC = src
DEBUG ?=

ifeq ($(DEBUG), 1)
	DBG = -DDEBUG
endif

OBJECTS = src/client.o \
		  src/client_tool.o\
		  src/server.o \
		  src/server_tool.o



all : client server

client :  src/client.o src/client_tool.o
	@echo "LD  $@"
	@$(CC) src/client.o src/client_tool.o $(DBG) -o  $@

server : src/server.o src/server_tool.o
	@echo "LD  $@"
	@$(CC) src/server.o src/server_tool.o $(DBG) -o  $@

$(SRC)/%.o : $(SRC)/%.c
	@echo " CC	$<"
	@$(CC) -c $(DBG) -I. $< -o $@

clean :
	rm client server src/*.o
