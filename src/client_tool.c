#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/socket.h>
#include <libxml/parser.h>
#include <include/client_tool.h>
char  *ipaddr, *port, *bin2run;

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


int is_leaf(xmlNode * node)
{
	xmlNode * child = node->children;
	while(child)
	{
		if(child->type == XML_ELEMENT_NODE) return 0;
		child = child->next;
	}
	return 1;
}

void print_xml(xmlNode * node, int indent_len)
{
	int i = 0;
	while(node)
	{
		if(node->type == XML_ELEMENT_NODE)
		{
			if (is_leaf(node)?xmlNodeGetContent(node):xmlGetProp(node, "id"))
			{
				switch(i) {
					case 0:
						ipaddr = is_leaf(node)?xmlNodeGetContent(node):xmlGetProp(node, "id");
						break;
					case 1:
						port = (is_leaf(node)?xmlNodeGetContent(node):xmlGetProp(node, "id"));
						break;
					case 2:
						bin2run = is_leaf(node)?xmlNodeGetContent(node):xmlGetProp(node, "id");
						break;
				}
				/* you can print all values here if i == 2, usefull for debug, I keep it here*/
				/*if (i == 2)
				{
					printf("%s\n", ipaddr);
					printf("%s\n", port);
					printf("%s\n", bin2run);
				}*/
			} i++;
		}
		print_xml(node->children, indent_len + 1);
		node = node->next;
    }
}