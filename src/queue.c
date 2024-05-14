#include <common.h>
#include <pthread.h>

Queue queue_push(Queue queue, struct Client *client){
    pthread_mutex_lock(&queue.mutex);

    client->next = NULL;

    if(queue.size == 0)
        queue.head = queue.tail = client;
    else{
        queue.tail->next = client;
        queue.tail = queue.tail->next;
    }

    queue.size++;

    pthread_mutex_unlock(&queue.mutex);
    return queue;
}

Queue queue_pop(Queue queue){
    pthread_mutex_lock(&queue.mutex);

    if(!queue.size)
        return queue;

    struct Client *tmp = queue.head;
    queue.head = queue.head->next;
    free(tmp);

    queue.size--;
    pthread_mutex_unlock(&queue.mutex);
    return queue;
}

Queue queue_remove(Queue queue, struct Client *client){
    pthread_mutex_lock(&queue.mutex);
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

    pthread_mutex_unlock(&queue.mutex);
    return queue;
}

void print_queue(Queue queue){
    pthread_mutex_lock(&queue.mutex);
    struct Client *current = queue.head;
    while(current != NULL){
        printf("%s --> ",current->username); 
        current = current->next;
    } 
    printf("\n");
    pthread_mutex_unlock(&queue.mutex);
    free(current);
}

