#include <stdio.h>
#include <stdlib.h>

#define MAX 20

typedef char ElemType;

typedef struct {
    ElemType data[MAX];
    int length;
} SqList;

void InitList(SqList *L)
{
    L->length = 0;
}

int Insert(SqList *L, int i, ElemType e)
{
    int j;
    if (i < 1 || i > L->length + 1 || L->length >= MAX)
        return 0;
    for (j = L->length; j >= i; j--)
        L->data[j] = L->data[j - 1];
    L->data[i - 1] = e;
    L->length++;
    return 1;
}

int Delete(SqList *L, int i, ElemType *e)
{
    int j;
    if (i < 1 || i > L->length)
        return 0;
    *e = L->data[i - 1];
    for (j = i; j < L->length; j++)
        L->data[j - 1] = L->data[j];
    L->length--;
    return 1;
}

int GetElem(SqList L, int i, ElemType *e)
{
    if (i < 1 || i > L.length)
        return 0;
    *e = L.data[i - 1];
    return 1;
}

int Locate(SqList L, ElemType e)
{
    int i;
    for (i = 0; i < L.length; i++)
        if (L.data[i] == e)
            return i + 1;
    return 0;
}

int Empty(SqList L)
{
    return L.length == 0;
}

void Print(SqList L)
{
    int i;
    printf("L:");
    for (i = 0; i < L.length; i++)
        printf(" %c", L.data[i]);
    printf("\n");
}

void DestroyList(SqList *L)
{
    L->length = 0;
}

int main(void)
{
    SqList L;
    ElemType e;
    int i;
    char s[5] = {'a', 'b', 'c', 'd', 'e'};

    InitList(&L);
    for (i = 0; i < 5; i++)
        Insert(&L, i + 1, s[i]);
    Print(L);
    printf("length=%d\n", L.length);
    printf("empty=%d\n", Empty(L));
    GetElem(L, 3, &e);
    printf("L[3]=%c\n", e);
    printf("pos(a)=%d\n", Locate(L, 'a'));
    Insert(&L, 4, 'f');
    Print(L);
    Delete(&L, 3, &e);
    Print(L);
    DestroyList(&L);
    printf("destroyed\n");
    return 0;
}
