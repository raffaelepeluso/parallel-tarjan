#include <stdio.h>
#include <stdlib.h>
#include "../include/SccStack.h"

SccStack SccStackCreate()
{
    SccStack stack;
    stack.top = NULL;
    stack.numSCC = 0;
    stack.totalIntegers = 0;
    stack.num_nodes = 0;
    return stack;
}

void SccStackDestroy(SccStack *stack)
{
    SccItem *temp;
    while (stack->top != NULL)
    {
        temp = stack->top;
        stack->top = temp->next;
        destroySCC(temp->scc);
        free(temp);
    }
}

void SccStackPush(SccStack *stack, SCC scc)
{
    SccItem *item = malloc(sizeof(SccItem));
    item->scc = scc;
    item->next = stack->top;
    stack->top = item;
    stack->numSCC++;
    stack->num_nodes += scc->num_nodes;
    stack->totalIntegers = stack->totalIntegers + scc->degree + scc->nodes.length + 2;
}

SCC SccStackPop(SccStack *stack)
{
    SccItem *temp = stack->top;
    SCC scc = temp->scc;
    stack->top = temp->next;
    free(temp);
    return scc;
}

SCC SccStackTop(SccStack *stack)
{
    return stack->top->scc;
}
int SccStackIsEmpty(SccStack *stack)
{
    return stack->top == NULL;
}

int *get_info(SccStack *stack)
{
    int size = stack->totalIntegers + 2;
    int *result = malloc(sizeof(int) * size);
    result[0] = size - 2;
    result[1] = stack->numSCC;
    int index = 2;
    while (!SccStackIsEmpty(stack))
    {
        SCC scc = SccStackPop(stack);
        result[index++] = scc->nodes.length;
        result[index++] = scc->degree;
        for (int i = 0; i < scc->nodes.length; i++)
        {
            result[index++] = scc->nodes.items[i]->id;
        }
        for (int i = 0; i < scc->nodes.length; i++)
        {
            for (int j = 0; j < scc->nodes.items[i]->degree; j++)
            {
                result[index++] = scc->nodes.items[i]->adj[j];
            }
        }
        destroySCC(scc);
    }
    return result;
}