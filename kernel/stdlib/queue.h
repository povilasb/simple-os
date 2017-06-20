#ifndef _QUEUE_H
#define _QUEUE_H

typedef struct structQueueElement
{
    void *data;
    struct structQueueElement *next;
} QueueElement;

typedef struct 
{
    QueueElement *front;
    QueueElement *rear;
} Queue;

void init_queue(Queue *queue);
int queue_add(Queue *queue, void *data);
void *queue_removeFirst(Queue *queue);
int queue_removeElement(Queue *queue, void *data);
int queue_size(Queue *queue);

#endif