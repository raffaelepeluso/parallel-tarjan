#ifndef SCC_STACK_H
#define SCC_STACK_H

#include "SCC.h"

typedef struct SccStackItem SccItem;

struct SccStackItem
{
    SCC scc;
    SccItem *next;
};

typedef struct
{
    SccItem *top;
    int numSCC, totalDegree, totalIntegers, num_nodes;
} SccStack;

SccStack SccStackCreate();
void SccStackDestroy(SccStack *stack);
void SccStackPush(SccStack *stack, SCC scc);
SCC SccStackPop(SccStack *stack);
SCC SccStackTop(SccStack *stack);
int SccStackIsEmpty(SccStack *stack);
int *get_info(SccStack *stack);

#endif