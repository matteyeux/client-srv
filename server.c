#define _GNU_SOURCE
#include <errno.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/socket.h>

#include <include/network.h>

int  serveur_tcp ();
int  leave_srv (void);
void manage_co (int sock);


int serveur_tcp(const char* service_port)
{
	int err;
	int sock_server;
	int curr_socket;
	struct addrinfo  hints;
	struct addrinfo *results;

	struct sockaddr_in* addr_in;
	socklen_t length = 0;
	char hostname [NI_MAXHOST];
	char servname [NI_MAXSERV];

	// Create srv socket & attribute service number 
	if ((sock_server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
		perror("socket");
		return -1;
	}

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	if ((err = getaddrinfo(NULL, service_port,  &hints, &results)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(err));
		close(sock_server);
		return -1;
	}

	err = bind(sock_server, results->ai_addr, results->ai_addrlen);
	freeaddrinfo(results);
	if (err < 0) {
		perror("bind");
		close(sock_server);
		return -1;
	}

	length = sizeof(struct sockaddr_in);
	if (getsockname(sock_server, (struct sockaddr *) &addr_in, &length) < 0) {
		perror ("getsockname");
		return -1;
	}
	if (getnameinfo((struct sockaddr *) &addr_in, length,
	                hostname, NI_MAXHOST,
	                servname, NI_MAXSERV,
	                NI_NUMERICHOST | NI_NUMERICSERV) == 0) 
		fprintf (stdout, "IP = %s, Port = %s \n", hostname, servname);

	listen(sock_server, 5);

	while (!leave_srv()) {
		curr_socket = accept(sock_server, NULL, NULL);
		if (curr_socket < 0) {
			perror("accept");
			return -1;
		}
		switch (fork()) {
			case 0 : // son
				close(sock_server);
				manage_co(curr_socket);
				exit(EXIT_SUCCESS);
			case -1 :
				perror("fork");
				return -1;
			default : // father
				close(curr_socket);
		}
	}
	return 0;
}

int leave_srv (void)
{
	return 0;
}


void manage_co (int sock)
{
	char buffer[256];
	int  buf_len;

	while (1) {
		buf_len = read(sock, buffer, 256);
		
		if (buf_len < 0) {
			perror("read");
			exit(EXIT_SUCCESS);
		}
		if (buf_len == 0)
			break;
		printf("%s",buffer);
		memset(buffer, 0, 256);
		buffer[buf_len] = '\0';
	}
	close(sock);
}

void usage(int argc, const char* argv[]){
	printf("usage : %s [port]\n", argv[0]);
}

int main (int argc, const char* argv[])
{
	const char* port;
	if (argc > 2)
	{
		usage(argc, argv);
		return 0;
	}

	if (argc == 1)
	{
		port = "50683";
	} else {
		if (!strcmp(argv[1], "-i"))
		{
			port = "50683";
			network_info();
		} else {
			usage(argc,argv);
		}
	}
	serveur_tcp(port);
	return 0;
}
