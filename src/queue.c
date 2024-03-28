#include <common.h>

Queue queue_push(Queue queue, struct Client *client){
    client->next = NULL;

    if(queue.size == 0)
        queue.head = queue.tail = client;
    else{
        queue.tail->next = client;
        queue.tail = queue.tail->next;
    }

    queue.size++;
    return queue;
}

Queue queue_pop(Queue queue){

    if(!queue.size)
        return queue;

    struct Client *tmp = queue.head;
    queue.head = queue.head->next;
    free(tmp);

    queue.size--;
    return queue;
}

Queue queue_remove(Queue queue, struct Client *client){
    if (queue.head->sc == client->sc) {
        queue.head = queue.head->next;
        return queue;
    }

    struct Client *current = queue.head;
    while (current->next != NULL) {
        if(current->next->sc == client->sc){
            struct Client *tmp = client;
            current->next = client->next;
            free(tmp); 
        }
        current = current->next;
    }
    free(current);
    queue.size--;
    return queue;
}

void print_queue(Queue queue){
    struct Client *current = queue.head;
    while(current != NULL){
        printf("%s --> ",current->username); 
        current = current->next;
    } 
    printf("\n");
    free(current);
}

