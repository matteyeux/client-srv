#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include <sys/socket.h>

#define LG_BUFFER	1024


int read_args (int server, char *host, char *port, struct addrinfo **result);
int exec_bin(int sock2server, const char *bin2exec);
void usage(int argc, char *argv[]);


int read_args (int server, char *host, char *port, struct addrinfo **results)
{
	int    err;	/*
	char *host  = "127.0.0.1";
	char *port = "4000";*/

	struct addrinfo  hints;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	if (server)
		hints.ai_flags = AI_PASSIVE;
	if ((err = getaddrinfo(host, port, &hints, results)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(err));
		return -1;
	}
	return 0;
}

int exec_bin(int sock2server, const char *bin2exec){
	
	int tube[2];
	char buf[1024];
	int n;
	if (pipe(tube) != 0){
		printf("err\n");
		exit(0);
	}

	switch(fork())
	{
		case -1:
			printf("err case -1\n");
			break;
		case 0: //son closes 1
			close(tube[0]);
			if (dup2(tube[1], STDOUT_FILENO) < 0) {
				perror("dup");
				exit(EXIT_FAILURE);
			}
			execl(bin2exec, "sysnet", "-n", NULL);
			perror("execlp");
			close(tube[1]);
			break;
		default: // father closes 0
			close(tube[1]);
			while(1){
				n = read(tube[0], buf, sizeof(buf));
				
				if (n > 0)
				{
					if (write(sock2server, buf, strlen(buf)) < 0) {
						perror("write");
						break;
					}
					memset(buf, 0, sizeof(buf));
				} else if (n == 0){
					break;
				} else {
					exit(1);
				}
			}
			close(tube[0]);
			break;
	}
	return 0;
}

void usage(int argc, char *argv[]){
	printf("usage : %s [ip] <port>\n",argv[0] );
}

int main (int argc, char *argv[])
{
	int    sock;
	char   buffer [LG_BUFFER];
	int    nb_lus;
	char *ipaddr, *port;
	struct addrinfo *results;
	
	if (argc != 2 && argc != 3)
	{
		usage(argc, argv);
		return 0;
	}
	if (argc == 2)
	{
		ipaddr = "127.0.0.1";
		port = argv[1];
	} else {
		ipaddr = argv[1];
		port = argv[2];
	}


	if (read_args(0,ipaddr, port, &results) < 0){
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
	exec_bin(sock, "/usr/local/bin/sysnet");
	close(sock);
	return 0;
}