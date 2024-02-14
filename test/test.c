#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "../include/tarjan.h"

void test_tarjan(int *expected_result, Graph *g);

int main(int argc, char *argv[])
{
    int g1_expected[] = {12, 9, 10, 11, 8, 3, 2, 1, 6, 4, 5, 7};
    int g2_expected[] = {3, 2, 1};

    // Connected graph with three SCC
    Graph *g1 = graphCreate(12);
    Node one = nodeCreate(1, 2);
    one->adj[0] = 2;
    one->adj[1] = 4;
    Node two = nodeCreate(2, 1);
    two->adj[0] = 3;
    Node three = nodeCreate(3, 2);
    three->adj[0] = 1;
    three->adj[1] = 7;
    Node four = nodeCreate(4, 1);
    four->adj[0] = 6;
    Node five = nodeCreate(5, 1);
    five->adj[0] = 4;
    Node six = nodeCreate(6, 1);
    six->adj[0] = 7;
    Node seven = nodeCreate(7, 1);
    seven->adj[0] = 5;
    Node eight = nodeCreate(8, 2);
    eight->adj[0] = 6;
    eight->adj[1] = 11;
    Node nine = nodeCreate(9, 1);
    nine->adj[0] = 8;
    Node ten = nodeCreate(10, 1);
    ten->adj[0] = 9;
    Node eleven = nodeCreate(11, 2);
    eleven->adj[0] = 10;
    eleven->adj[1] = 12;
    Node twelve = nodeCreate(12, 2);
    twelve->adj[0] = 7;
    twelve->adj[1] = 10;
    addNode(g1, one, 0);
    addNode(g1, two, 1);
    addNode(g1, three, 2);
    addNode(g1, four, 3);
    addNode(g1, five, 4);
    addNode(g1, six, 5);
    addNode(g1, seven, 6);
    addNode(g1, eight, 7);
    addNode(g1, nine, 8);
    addNode(g1, ten, 9);
    addNode(g1, eleven, 10);
    addNode(g1, twelve, 11);

    // Disconnected graph with one SCC
    Graph *g2 = graphCreate(3);
    Node node1 = nodeCreate(1, 1);
    node1->adj[0] = 2;
    Node node2 = nodeCreate(2, 1);
    node2->adj[0] = 1;
    Node node3 = nodeCreate(3, 0);
    addNode(g2, node1, 0);
    addNode(g2, node2, 1);
    addNode(g2, node3, 2);

    test_tarjan(g1_expected, g1);
    test_tarjan(g2_expected, g2);

    graphDestroy(g1);
    graphDestroy(g2);

    return 0;
}

void test_tarjan(int *expected_result, Graph *g)
{
    int index = 0;
    SccStack actual_result = tarjan(g);
    while (!SccStackIsEmpty(&actual_result))
    {
        SCC s = SccStackPop(&actual_result);
        for (int i = 0; i < s->nodes.length; i++)
        {
            assert(expected_result[index] == s->nodes.items[i]->id);
            index++;
        }
        destroySCC(s);
    }
    SccStackDestroy(&actual_result);
}