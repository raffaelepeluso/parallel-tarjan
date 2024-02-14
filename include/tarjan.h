#ifndef TARJAN_H
#define TARJAN_H

#include <stdbool.h>

#include "graph.h"
#include "SccStack.h"
#include "stack.h"

void SCCUtil(Graph *g, Node currentNode, Stack *stack, SccStack *results);
SccStack tarjan(Graph *g);

#endif