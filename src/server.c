#include <common.h>

void parse_packet(char* packet, char *code, char* message);
void chat(struct Client *client);
void broadcast_message(struct Client *client, char* message);
void thread_connection(struct Client *client);
int connection(int sc);

Queue clients;

int main(){
    printf("welcome to chatapp\n");
    int sc = socket(AF_INET, SOCK_STREAM,0);
    if(0 > sc){
        fprintf(stderr,"Error initializing socket");
        return -1;
    }

    struct sockaddr_in *server_address = create_address("", 1337);
    if(0 > bind(sc, (struct sockaddr*) server_address, sizeof(*server_address))){
        fprintf(stderr,"Error binding socket");
        return -1;
    }

    if(0 > listen(sc, 10)){
        fprintf(stderr,"Error initializing listen");
        return -1;
    }
    printf("server listening on port 1337\n");

    clients.size = 0;
    clients.head = clients.tail = NULL;

    while(1){
        struct Client *client = malloc(sizeof(struct Client));
        client->sc = connection(sc);
        // printf("connected to client %d\n", client->sc);
        clients = queue_push(clients, client);
        thread_connection(client);
    }

    shutdown(sc, SHUT_RDWR);
    close(sc);
}

int connection(int sc){
    struct sockaddr_in *client_addr= malloc(sizeof(struct sockaddr_in));
    socklen_t client_addr_size = sizeof(struct sockaddr_in);
    int clientsc = accept(sc, (struct sockaddr*) client_addr, &client_addr_size);
    if(0 > clientsc){
        fprintf(stderr,"Error connecting to client");
        return -1;
    }
    free(client_addr);
    return clientsc;
}

void thread_connection(struct Client *client){
    // printf("creating thread\n");
    pthread_t thread_id;
    pthread_create(&thread_id, NULL, (void*) chat, client);
}

void chat(struct Client *client){
    char *buffer = malloc(1024);
    char *message = malloc(1024);
    char *code = malloc(4);

    size_t char_count;
    while((char_count = recv(client->sc, buffer, 1024, 0)) > 0 ){
        // printf("packet received: %s\nsize: %d\n", buffer, (int)char_count);
        parse_packet(buffer, code, message);

        if (!strcmp(code, LOGIN)) {
            strncpy(client->username, message, 30);
            sprintf(message,"--------- %s JOINED ---------\n", client->username);
            send(client->sc, SUCCESS, CODE_LEN, 0);
            broadcast_message(client, message);
        }else if (!strcmp(code, EXIT)) {
            sprintf(message,"--------- %s LEFT ---------\n", client->username);
            send(client->sc, EXIT, CODE_LEN, 0);
            broadcast_message(client, message);
            print_queue(clients);
            close(client->sc);
            clients = queue_remove(clients, client);
            return;
        }else if (!strcmp(code, SUCCESS)) {
            buffer[char_count] = 0;
            char* packet = malloc(1055);
            sprintf(packet,"%s: %s\n", client->username, message);
            broadcast_message(client, packet);
            free(packet);
        }
    }

    close(client->sc);
    free(buffer);
    free(message);
}

void broadcast_message(struct Client *client, char* packet){
    struct Client *current = NULL;
    current = clients.head;
    printf("sending: %s ", packet);
    while (current != NULL) {
        if (current->sc != client->sc) {
            printf("    to : %s\n", current->username);
            send(current->sc, packet, strlen(packet), 0);
        } 
        current = current->next;
    }
    free(current);
}

void parse_packet(char* packet, char *code, char* message){

    if (strnlen(packet,1024+CODE_LEN+1)<CODE_LEN) 
        return;

    char *delimiter = ":";
    char *pos = strstr(packet, delimiter);

    if (pos){
        *pos = '\0';
        strncpy(code, packet, 4);
        pos++;
        strncpy(message, pos, 1024);
        return;
    }

    strncpy(code, packet, CODE_LEN);
}
