#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include <sys/socket.h>
#include <include/client_tool.h>

void usage(int argc, char *argv[]){
	printf("usage : %s -i [ip] -p [port] -c <cmd>\n",argv[0]);
	printf("-i, --ip [ip]\t\t set ip server\n");
	printf("-p, --port [port]\t set port server\n");
	printf("-c, --cmd [cmd]\t\t set command to run\n");
}

int main (int argc, char *argv[])
{
	int    sock;
	char  *ipaddr = "127.0.0.1", *port = "50683", *bin2run = "ls";
	struct addrinfo *results;
	
	if (argc < 2 || argc > 6)
	{
		usage(argc, argv);
		return 0;
	}

	for(int i = 0; i < argc; i++) {
		if(!strcmp(argv[i], "-i") || !strcmp(argv[i], "--ip")) {
			ipaddr = argv[i + 1];
		} else if(!strcmp(argv[i], "-p") || !strcmp(argv[i], "--port")) {
			port = argv[i + 1];
		} else if (!strcmp(argv[i], "-c") || !strcmp(argv[i], "--cmd")) {
			bin2run = argv[i + 1];
		} else {
			usage(argc, argv);
			return 0;
		}
	}

	if (init_client(0,ipaddr, port, &results) < 0){
		exit(EXIT_FAILURE);
	}
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket");
		exit(EXIT_FAILURE);
	}
	if (connect(sock, results->ai_addr, results->ai_addrlen) < 0) {
		perror("connect");
		exit(EXIT_FAILURE);
	}
	freeaddrinfo(results);
	exec_bin(sock, bin2run);
	close(sock);
	return 0;
}
