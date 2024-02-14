#ifndef NODE_H
#define NODE_H

#include <stdbool.h>

typedef struct
{
    int id, degree, low, disc;
    bool stackMember, superNode;
    int *adj;
    int *components;
    int dim;
} TNode;

typedef TNode *Node;

Node nodeCreate(int id, int degree);
Node superNodeCreate(int id, int degree, int *components, int dim);
void nodeDestroy(Node node);
int nodeEqual(Node node1, Node node2);
int nodeGreater(Node node1, Node node2);
int nodeLess(Node node1, Node node2);

#endif