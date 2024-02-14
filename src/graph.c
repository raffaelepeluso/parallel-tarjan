#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <limits.h>

#include "../include/graph.h"

Graph *graphCreate(int v)
{
    Graph *g = malloc(sizeof(Graph));
    g->V = v;
    g->Time = 0;
    g->maxId = -1;
    g->minId = INT_MAX;
    g->nodes = malloc(sizeof(Node) * v);
    return g;
}

void addNode(Graph *g, Node node, int index)
{
    g->nodes[index] = node;
    if (node->id > g->maxId)
        g->maxId = node->id;
    if (node->id < g->minId)
        g->minId = node->id;
}

void graphDestroy(Graph *g)
{
    for (int i = 0; i < g->V; i++)
        nodeDestroy(g->nodes[i]);
    free(g);
}

Node nodeSearch(Graph *g, int nodeId)
{
    Node node = nodeCreate(nodeId, 0);
    int low = 0;
    int high = g->V - 1;

    while (low <= high)
    {
        int mid = low + (high - low) / 2;

        if (nodeEqual(g->nodes[mid], node))
        {
            nodeDestroy(node);
            return g->nodes[mid];
        }

        if (nodeLess(g->nodes[mid], node))
            low = mid + 1;
        else
            high = mid - 1;
    }
    nodeDestroy(node);
    return NULL;
}

Graph *from_buffer(int *buffer, int num_vertices)
{
    int bp = 0;
    int id, degree, target;
    Graph *graph = graphCreate(num_vertices);
    for (int i = 0; i < num_vertices; i++)
    {
        id = buffer[bp++];
        degree = buffer[bp++];
        Node node = nodeCreate(id, degree);
        for (int j = 0; j < degree; j++)
        {
            target = buffer[bp++];
            node->adj[j] = target;
        }
        addNode(graph, node, i);
    }
    return graph;
}
