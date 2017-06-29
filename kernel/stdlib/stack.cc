/*
    Dependant from kmalloc and kfree.
*/

#include "stack.h"
#include "stdlib.h"
#include "stdio.h"
#include "../kernel/kernelHeap.h"

void init_stack(Stack *s)
{
    s->head = NULL;
}

int stack_push(Stack *s, void *data)
{
    StackElement *stackE;

    stackE = (StackElement*)kmalloc(sizeof(StackElement));
    if (stackE == NULL)
        return 0; //failure

    stackE->data = data;
    stackE->next = s->head;
    s->head = stackE;

    return 1; //success
}

void *stack_pop(Stack *s)
{
    void *data;
    StackElement *temp;

    if (s->head == NULL)
        return NULL;

    data = s->head->data;
    temp = s->head;
    s->head = s->head->next;
    kfree(temp);

    return data;
}

int stack_removeElement(Stack *s, void *data)
{
    StackElement *temp, *prev;

    temp = s->head;
    if (temp == NULL)
        return FALSE;

    if (temp->data == data)
    {
        stack_pop(s);
        return TRUE;
    }

    while (temp != NULL)
    {
        if (temp->data == data)
        {
            prev->next = temp->next;
            kfree(temp);
            return TRUE;
        }
        prev = temp;
        temp = temp->next;
    }

    return FALSE;
}

void stack_print(Stack *s)
{
    StackElement *temp;

    temp = s->head;
    while (temp != NULL)
    {
        kprintf("%d = 0x%x\n", temp->data, temp->data);
        temp = temp->next;
    }
}
