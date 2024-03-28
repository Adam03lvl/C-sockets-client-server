#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SUCCESS "200"
#define EXIT "404"
#define LOGIN "101"
#define CODE_LEN 3

struct sockaddr_in *create_address(char* ipv4, int port);
