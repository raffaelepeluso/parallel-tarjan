#ifndef GRAPH_H
#define GRAPH_H

#include "node.h"

typedef struct
{
    int V, Time;
    int maxId, minId;
    Node *nodes;
} Graph;

Graph *graphCreate(int v);
void addNode(Graph *g, Node node, int index);
void graphDestroy(Graph *g);
Node nodeSearch(Graph *g, int nodeId);
Graph *from_buffer(int *buffer, int num_vertices);

#endif