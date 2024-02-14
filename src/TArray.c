#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "../include/TArray.h"

#define C_EXP 50000
#define C_RED 100000

TArray arrayCreate(int length)
{
    TArray a;
    int size = length + C_EXP;
    a.items = malloc(size * sizeof(Node));
    assert(a.items != NULL);
    a.length = length;
    a.size = size;
    return a;
}

void arrayDestroy(TArray *a)
{
    free(a->items);
    a->items = NULL;
    a->length = a->size = 0;
}

void arrayResize(TArray *a, int length)
{
    if (length > a->size || length < a->size - C_RED)
    {
        int size = length + C_EXP;
        a->items = realloc(a->items, size * sizeof(int));
        assert(a->items != NULL);
        a->size = size;
    }
    a->length = length;
}

void arrayAdd(TArray *a, Node info)
{
    arrayResize(a, a->length + 1);
    a->items[a->length - 1] = info;
}

Node arrayGet(TArray *a, int pos)
{
    return a->items[pos];
}

int arrayLength(TArray *a)
{
    return a->length;
}

void arraySet(TArray *a, int pos, Node info)
{
    if (pos >= a->length)
        arrayResize(a, pos + 1);
    a->items[pos] = info;
}

bool IdNotIn(TArray *a, int id)
{
    for (int i = 0; i < a->length; i++)
        if (a->items[i]->id == id)
            return true;
    return false;
}

void arraySort(TArray *a)
{
    int i, j;
    Node key;
    for (i = 1; i < a->length; i++)
    {
        key = a->items[i];
        j = i - 1;

        while (j >= 0 && nodeGreater(a->items[j], key))
        {
            a->items[j + 1] = a->items[j];
            j = j - 1;
        }
        a->items[j + 1] = key;
    }
}

bool idSearch(TArray *a, int nodeId)
{
    arraySort(a);
    Node node = nodeCreate(nodeId, 0);
    int low = 0;
    int high = a->length - 1;

    while (low <= high)
    {
        int mid = low + (high - low) / 2;

        if (nodeEqual(a->items[mid], node))
        {
            nodeDestroy(node);
            return true;
        }

        if (nodeLess(a->items[mid], node))
            low = mid + 1;
        else
            high = mid - 1;
    }
    nodeDestroy(node);
    return false;
}