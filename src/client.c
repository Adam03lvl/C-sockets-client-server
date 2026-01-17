#include <common.h>

int login(int sc);
int messaging(int sc);
int send_message(int sc);
int receive_message(int sc);
char *generate_packet(char* code, char* message, int *packet_count);

int main(int argc, char** argv){
    printf("welcome to chatapp\n");

    if(argc != 2){
        perror("USAGE: client <SERVER_ADDRESS>");
        return -1;
    }

    int sc = socket(AF_INET, SOCK_STREAM,0);
    if(0 > sc){
        perror("Error opening socket");
        return -1;
    }

    struct sockaddr_in *server_address = create_address(argv[1],PORT);

    socklen_t server_addr_len = (socklen_t) sizeof(*server_address);

    if(0 > connect(sc, (struct sockaddr *) server_address, server_addr_len)){
        perror("Error connecting");
        return -1;
    }
    printf("connection succesful\n");

    if(0 > login(sc)){
        fprintf(stderr,"COULDN'T LOG IN");
        exit(-1);
    }

    if(0 > messaging(sc)){
        fprintf(stderr,"SESSION CLOSED UNEXPECTEDLY");
        exit(-1);
    }
    close(sc);
}

int login(int sc){
    int packet_count = 0;
    size_t char_count = 0;
    char *username = malloc(char_count);
    char *packet = malloc(char_count);
    char *error = malloc(2*CODE_LEN);

    printf("enter username: ");
    if(0 > getline(&username, &char_count, stdin)){
        return -1;
    }

    packet = generate_packet(LOGIN, username, &packet_count);

    if(0 > send(sc, packet, packet_count, 0)){
        return -1;
    }

    free(username);
    free(packet);

    recv(sc, error, CODE_LEN, 0);
    if (strncmp(error, SUCCESS, CODE_LEN))
        return 0; 

    return 1;
}

int messaging(int sc){
    pthread_t send, receive;

    int send_return;
    int recv_return;

    pthread_create(&send, NULL, (void*) send_message, (void*) sc);
    pthread_create(&receive, NULL, (void*) receive_message, (void*) sc);

    pthread_join(receive, (void**) &recv_return);
    pthread_join(send, (void**) &send_return);

    if(0 > send_return){
        return -1;
    }
    if(0 > recv_return){
        return -1;
    }

    return 0;
}

int send_message(int sc){
    char *message, *error, *packet;
    int packet_length = 0;
    size_t line_size = 0;
    do{
        size_t char_count = getline(&message, &line_size, stdin);

        if (!strncmp(message, "exit\n", 6)){
            send(sc, EXIT, CODE_LEN, 0);
            printf("connection closed\n");
            break;
        }

        packet = generate_packet(SUCCESS, message, &packet_length);
        // printf("packet sent: %s\nsize: %d\n", packet, (int)packet_length);

    }while(send(sc, packet, packet_length, 0)>0);
    return 0;
}

int receive_message(int sc){
    char *message, *error, *packet;
    char *buffer = malloc(1024);
    size_t char_count;

    while ((char_count = recv(sc, buffer, 1024, 0)) >= CODE_LEN) {
        buffer[char_count] = 0;
        printf("%s", buffer); 
        if(!strncmp(buffer, EXIT, CODE_LEN)){
            printf("connection closed");
            break;
        }
    }

    free(buffer);
    return 0;
}

char *generate_packet(char* code, char* message, int *packet_count){
    *packet_count = strlen(message)+CODE_LEN+1;
    char *packet_buffer = malloc(*packet_count);
    snprintf(packet_buffer, *packet_count,"%s:%s", code, message);
    return packet_buffer;
}
