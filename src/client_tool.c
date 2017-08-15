#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/socket.h>

int init_client (int server, char *host, char *port, struct addrinfo **results)
{
	int    err;	
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

int exec_bin(int sock2server, const char* bin2exec){
	
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
		case 0: 
			close(tube[0]);
			if (dup2(tube[1], STDOUT_FILENO) < 0) {
				perror("dup");
				exit(EXIT_FAILURE);
			}
			
			system(bin2exec);
			#ifdef DEBUG
				perror("system");
			#endif
			close(tube[1]);
			break;
		default: 
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
