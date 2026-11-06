#include <stdio.h>

#define MAX 20

typedef struct {
    int data[MAX];
    int length;
} SqList;

void InitList(SqList *L)
{
    L->length = 0;
}

int Insert(SqList *L, int i, int e)
{
    int j;
    if (i < 1 || i > L->length + 1 || L->length == MAX)
        return 0;
    for (j = L->length; j >= i; j--)
        L->data[j] = L->data[j - 1];
    L->data[i - 1] = e;
    L->length++;
    return 1;
}

int Delete(SqList *L, int i, int *e)
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

void Print(SqList L)
{
    int i;
    for (i = 0; i < L.length; i++)
        printf("%d%s", L.data[i], i == L.length - 1 ? "\n" : " ");
}

int main(void)
{
    SqList L;
    int i, e;

    InitList(&L);
    for (i = 1; i <= 10; i++)
        Insert(&L, i, i);
    Delete(&L, 5, &e);
    Print(L);
    printf("length=%d\n", L.length);
    return 0;
}
