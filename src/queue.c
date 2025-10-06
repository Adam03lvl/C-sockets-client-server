#include <common.h>

Queue queue_push(Queue queue, Client *client){
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

    Client *tmp = queue.head;
    queue.head = queue.head->next;
    free(tmp);

    queue.size--;
    pthread_mutex_unlock(&queue.mutex);
    return queue;
}

Queue queue_remove(Queue queue, Client *client){
    pthread_mutex_lock(&queue.mutex);
    
    if (queue.head && queue.head->sc == client->sc) {
        Client *tmp = queue.head;
        queue.head = queue.head->next;
        if (queue.head == NULL)
            queue.tail = NULL;
        free(tmp);
        queue.size--;
        pthread_mutex_unlock(&queue.mutex);
        return queue;
    }
    
    Client *current = queue.head;
    while (current && current->next) {
        if(current->next->sc == client->sc){
            Client *tmp = current->next;
            current->next = tmp->next;
            if (tmp == queue.tail)
                queue.tail = current;
            free(tmp);
            queue.size--;
            break;
        }
        current = current->next;
    }
    
    pthread_mutex_unlock(&queue.mutex);
    return queue;
}

void print_queue(Queue queue){
    Client *current = queue.head;
    while(current != NULL){
        printf("%s --> ",current->username); 
        current = current->next;
    } 
    printf("\n");
}
