#define LG_BUFFER	1024

int init_client (int server, char *host, char *port, struct addrinfo **result);
int exec_bin(int sock2server, const char* bin2exec);
