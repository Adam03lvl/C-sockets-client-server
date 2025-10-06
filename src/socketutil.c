#include <common.h>

struct sockaddr_in *create_address(char* ipv4, int port){
    struct sockaddr_in *address = malloc(sizeof(struct sockaddr_in));
    address->sin_family = AF_INET; 
    address->sin_port = htons(port);
    if (strlen(ipv4) == 0) {
        address->sin_addr.s_addr = INADDR_ANY; 
    }else 
        inet_pton(AF_INET, ipv4, &address->sin_addr.s_addr);

    return address;
}
