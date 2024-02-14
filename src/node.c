#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../include/node.h"

Node nodeCreate(int id, int degree)
{
    Node n = malloc(sizeof(TNode));
    n->id = id;
    n->degree = degree;
    n->disc = -1;
    n->low = -1;
    n->stackMember = false;
    n->superNode = false;
    n->components = NULL;
    n->dim = -1;
    if (degree >= 0)
        n->adj = malloc(sizeof(int) * degree);
    else
        n->adj = NULL;
    return n;
}

Node superNodeCreate(int id, int degree, int *components, int dim)
{
    Node n = malloc(sizeof(TNode));
    n->id = id;
    n->degree = degree;
    n->disc = -1;
    n->low = -1;
    n->stackMember = false;
    n->superNode = true;
    n->components = components;
    n->dim = dim;
    if (degree >= 0)
        n->adj = malloc(sizeof(int) * degree);
    else
        n->adj = NULL;
    return n;
}

void nodeDestroy(Node node)
{
    if (node->superNode)
        free(node->components);
    free(node->adj);
    free(node);
}

int nodeEqual(Node node1, Node node2)
{
    return node1->id == node2->id;
}

int nodeGreater(Node node1, Node node2)
{
    return node1->id > node2->id;
}

int nodeLess(Node node1, Node node2)
{
    return node1->id < node2->id;
}
