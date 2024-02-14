#ifndef TARRAY_H
#define TARRAY_H

#include "stdbool.h"
#include "node.h"

typedef struct
{
    Node *items;
    int length;
    int size;
} TArray;

TArray arrayCreate(int length);
void arrayDestroy(TArray *a);
void arrayResize(TArray *a, int length);
void arrayAdd(TArray *a, Node info);
Node arrayGet(TArray *a, int pos);
int arrayLength(TArray *a);
void arraySet(TArray *a, int pos, Node info);
bool idSearch(TArray *a, int nodeId);
void arraySort(TArray *a);

#endif