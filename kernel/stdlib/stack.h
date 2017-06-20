#ifndef _STACK_H
#define _STACK_H

typedef struct _StackElement
{
    void *data;
    struct _StackElement *next;
} StackElement;

typedef struct
{
    StackElement *head;
} Stack;

void init_stack(Stack *s);
int stack_push(Stack *s, void *data);
void *stack_pop(Stack *s);
int stack_removeElement(Stack *s, void *data);
void stack_print(Stack *s);

#endif