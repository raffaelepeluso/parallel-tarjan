#ifndef SCC_H
#define SCC_H

#include "TArray.h"
#include "node.h"

typedef struct
{
    TArray nodes;
    int degree, num_nodes;
} SuperNode;

typedef SuperNode *SCC;

SCC createSCC();
void destroySCC(SCC scc);
void addToSCC(SCC scc, Node node);

#endif