#include <stdio.h>
#include <stdlib.h>
#include "../include/stack.h"

Stack stackCreate()
{
    Stack stack;
    stack.top = NULL;
    return stack;
}

void stackDestroy(Stack *stack)
{
    Item *temp;
    while (stack->top != NULL)
    {
        temp = stack->top;
        stack->top = temp->next;
        free(temp);
    }
}

void stackPush(Stack *stack, Node node)
{
    Item *item = malloc(sizeof(Item));
    item->node = node;
    item->next = stack->top;
    stack->top = item;
}

Node stackPop(Stack *stack)
{
    Item *temp = stack->top;
    Node node = temp->node;
    stack->top = temp->next;
    free(temp);
    return node;
}
Node stackTop(Stack *stack)
{
    return stack->top->node;
}
int stackIsEmpty(Stack *stack)
{
    return stack->top == NULL;
}