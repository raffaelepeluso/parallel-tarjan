#include <stdio.h>
#include <string.h>
#include "../include/TInfo_KV.h"

int infoEqual(TInfo info1, TInfo info2)
{
    return info1.key == info2.key;
}

int infoGreater(TInfo info1, TInfo info2)
{
    return info1.key > info2.key;
}

int infoLess(TInfo info1, TInfo info2)
{
    return info1.key < info2.key;
}

void infoPrint(TInfo info)
{
    printf("(%d: %d) ", info.key, info.value);
}

unsigned int keyHash(TKey key)
{
    key = ((key >> 16) ^ key) * 0x45d9f3b;
    key = ((key >> 16) ^ key) * 0x45d9f3b;
    key = (key >> 16) ^ key;
    return key;
}