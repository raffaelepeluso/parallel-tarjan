#include <stdio.h>
#include <stdlib.h>
#include "../include/SCC.h"

SCC createSCC()
{
    SCC scc = malloc(sizeof(SuperNode));
    scc->nodes = arrayCreate(0);
    scc->degree = 0;
    scc->num_nodes = 0;
    return scc;
}

void destroySCC(SCC scc)
{
    arrayDestroy(&scc->nodes);
    free(scc);
}

void addToSCC(SCC scc, Node node)
{
    arrayAdd(&scc->nodes, node);
    scc->degree += node->degree;
    if (node->superNode)
    {
        scc->num_nodes += node->dim;
    }
    else
    {
        scc->num_nodes++;
    }
}
