#include <libxml/parser.h>

#define LG_BUFFER	1024

struct addrinfo *results;
char  *ipaddr, *port, *bin2run;

int init_client (int server, char *host, char *port, struct addrinfo **result);
int exec_bin(int sock2server, const char* bin2exec);
int is_leaf(xmlNode * node);
void print_xml(xmlNode * node, int indent_len);