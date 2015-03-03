/*
    Dependant from kmalloc and kfree.
*/

#include "queue.h"
#include "stdlib.h"
#include "../kernel/kernelHeap.h"

void init_queue(Queue *queue)
{
    queue->front = NULL;
    queue->rear = NULL;
}

int queue_add(Queue *queue, void *data)
{
    QueueElement *element;
    
    element = kmalloc(sizeof(QueueElement));
    if (element == NULL)
        return 0; //failure
        
    element->data = data;
    element->next = NULL;
        
    if (queue->front == NULL)
    {
        queue->front = element;
        queue->rear = element;
    }
    else
    {
        queue->rear->next = element;
        queue->rear = element;
    }
    
    return 1; //success
}

void *queue_removeFirst(Queue *queue)
{
    QueueElement *temp;
    void *data;
    
    temp = queue->front;
    if (temp == NULL)
        return NULL;
    queue->front = queue->front->next;
    data = temp->data;
    kfree(temp);
    
    if (queue->front == NULL)
        queue->rear = NULL;
        
    return data;
}

int queue_removeElement(Queue *queue, void *data)
{
    QueueElement *temp, *prev;
    
    temp = queue->front;
    if (temp == NULL)
        return;
        
    if (temp->data == data)
    {
        queue_removeFirst(queue);
        return TRUE;
    }
    
    while (temp != NULL)
    {
        if (temp->data == data)
        {
            prev->next = temp->next;
            if (prev->next == NULL)
                queue->rear = prev;
            kfree(temp);
            return TRUE;
        }
        prev = temp;
        temp = temp->next;
    }
    
    return FALSE;
}

int queue_size(Queue *queue)
{
    QueueElement *element;
    int size = 0;
    
    element = queue->front;
    while (element != NULL)
    {
        element = element->next;
        size++;
    }
    
    return size;
}