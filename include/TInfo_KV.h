#ifndef TINFO_H
#define TINFO_H

typedef int TKey;
typedef int TValue;

typedef struct
{
    TKey key;
    TValue value;
} TInfo;

int infoEqual(TInfo, TInfo);
int infoGreater(TInfo, TInfo);
int infoLess(TInfo, TInfo);
void infoPrint(TInfo);
unsigned int keyHash(TKey);

#endif
