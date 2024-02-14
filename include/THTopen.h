#ifndef THT_H
#define THT_H
#include "TInfo_KV.h"

#define MAX_LOAD 0.75
#define GROW_FACTOR 2

typedef struct SHashTable
{
    TInfo *bucket;
    int *used;
    int n_bucket;
    int n_used;
} THT;

THT *HTcreate(int n);
void HTdestroy(THT *ht);
void HTinsert(THT *ht, TKey key, TValue value);
void HTdelete(THT *ht, TKey key);
TValue *HTsearch(THT *ht, TKey key);
void HTprint(THT *ht);
void HTprintStructure(THT *ht);

#endif