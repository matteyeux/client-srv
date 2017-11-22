#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>

#include <sys/socket.h>
#include <include/client_tool.h>
#include <libxml/parser.h>

void usage(int argc, char *argv[]){
	printf("usage : %s -i [ip] -p [port] -c <cmd>\n",argv[0]);
	printf("-i, --ip [ip]\t\t set ip server\n");
	printf("-p, --port [port]\t set port server\n");
	printf("-c, --cmd [cmd]\t\t set command to run\n");
	printf("-x, --xml [xmlfile]\t set parameters in xml file\n");
	printf("-h, --help\t\t print help\n");
}

int main (int argc, char *argv[])
{
	int    sock;
	int i, xml = 0;

	char *xmlfile = NULL;
	if (argc > 6 || argc <= 2)
	{
		usage(argc, argv);
		return 0;
	}

	for(i = 0; i < argc; i++) {
		if(!strcmp(argv[i], "-i") || !strcmp(argv[i], "--ip")) {
			ipaddr = argv[i + 1];
		} else if(!strcmp(argv[i], "-p") || !strcmp(argv[i], "--port")) {
			port = argv[i + 1];
		} else if (!strcmp(argv[i], "-c") || !strcmp(argv[i], "--cmd")) {
			bin2run = argv[i + 1];
		} else if (!strcmp(argv[i], "-x") || !strcmp(argv[i], "--xml"))
		{
			xml = 1;
			xmlfile = argv[i+1];
		} else if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")) {
			usage(argc, argv);
			return 0;
		}
	}
	if (xml)
	{
		if (xmlfile == NULL || access(xmlfile, F_OK) == -1)
		{
			fprintf(stderr, "%s : %s\n", xmlfile, strerror(errno));
			return -1;
		} else {
			xmlDoc *doc = NULL;
			xmlNode *root_element = NULL;
			doc = xmlReadFile(xmlfile, NULL, 0);

			if (doc == NULL) {
				printf("Could not parse the XML file");
			}

			root_element = xmlDocGetRootElement(doc);
			print_xml(root_element, 1);
			xmlFreeDoc(doc);
			xmlCleanupParser();
			printf("%s\n", ipaddr);
			printf("%s\n", port);
			printf("%s\n", bin2run);
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

