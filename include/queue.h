struct Client{
    int sc;
    char username[30];
    struct Client *next; 
};

typedef struct Queue{ 
    struct Client *head;
    struct Client *tail;
    int size;
    pthread_mutex_t mutex;
}Queue;

Queue queue_push(Queue queue, struct Client *client);

Queue queue_pop(Queue queue);

Queue queue_remove(Queue queue, struct Client *client);

void print_queue(Queue queue);
