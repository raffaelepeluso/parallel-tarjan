#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "../include/tarjan.h"

void SCCUtil(Graph *g, Node currentNode, Stack *stack, SccStack *results)
{
    currentNode->disc = g->Time;
    currentNode->low = g->Time;
    g->Time += 1;
    currentNode->stackMember = true;
    stackPush(stack, currentNode);

    Node nextNode;

    for (int i = 0; i < currentNode->degree; i++) // controllo taglio
    {

        if (currentNode->adj[i] <= g->maxId && currentNode->adj[i] >= g->minId)
        {
            nextNode = nodeSearch(g, currentNode->adj[i]); // O(log n) -> n = numero nodi del grafo (binary search)
            if (nextNode->disc == -1)
            {
                SCCUtil(g, nextNode, stack, results);
                currentNode->low = (currentNode->low <= nextNode->low) ? currentNode->low : nextNode->low;
            }
            else if (nextNode->stackMember)
            {
                currentNode->low = (currentNode->low <= nextNode->disc) ? currentNode->low : nextNode->disc;
            }
        }
    }
    if (currentNode->low == currentNode->disc)
    {
        int w = -1;
        SCC scc = createSCC();
        while (w != currentNode->id)
        {
            Node node = stackPop(stack);
            node->stackMember = false;
            w = node->id;
            addToSCC(scc, node);
        }
        SccStackPush(results, scc);
    }
}

SccStack tarjan(Graph *g)
{
    Stack stack = stackCreate();
    SccStack result = SccStackCreate();

    for (int i = 0; i < g->V; i++)
    {
        Node currentNode = g->nodes[i];
        if (currentNode->disc == -1)
            SCCUtil(g, currentNode, &stack, &result);
    }

    stackDestroy(&stack);

    return result;
}
