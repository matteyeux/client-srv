#define _GNU_SOURCE
#include <errno.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/socket.h>
#include <include/server_tool.h>

void usage(int argc, const char* argv[]){
	printf("usage : %s [port]\n", argv[0]);
}

int main (int argc, const char* argv[])
{
	const char* port;

	if (argc == 1) {
		port = "50683";
	} else if (argc == 2) {
		port = argv[1];
	} else {
		usage(argc,argv);
		return -1;
	}
	tcp_server(port);
	return 0;
}
