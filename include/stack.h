#ifndef STACK_H
#define STACK_H

#include "node.h"

typedef struct stackItem Item;

struct stackItem
{
    Node node;
    Item *next;
};

typedef struct
{
    Item *top;
} Stack;

Stack stackCreate();
void stackDestroy(Stack *stack);
void stackPush(Stack *stack, Node node);
Node stackPop(Stack *stack);
Node stackTop(Stack *stack);
int stackIsEmpty(Stack *stack);

#endif
