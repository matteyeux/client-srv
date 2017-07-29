#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include <sys/socket.h>
#include <include/client_tool.h>

/*void usage(int argc, char *argv[]);

*/

void usage(int argc, char *argv[]){
	printf("usage : %s [ip] [port] <cmd>\n",argv[0]);
}

int main (int argc, char *argv[])
{
	int    sock;
	char   buffer [LG_BUFFER];
	int    nb_lus;
	char  *ipaddr, *port, *bin2run;
	struct addrinfo *results;
	
	if (argc < 2 || argc > 4)
	{
		usage(argc, argv);
		return 0;
	}
	// laziness
	switch(argc){
		case 2:
			ipaddr = "127.0.0.1";
			port = "50683";
			bin2run = argv[1];
			break;
		case 3:
			ipaddr = argv[1];
			port = "50683";
			bin2run = argv[2];
			break;
		case 4:
			ipaddr = argv[1];
			port = argv[2];
			bin2run = argv[3];
			break;
		default:
			usage(argc, argv);
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
