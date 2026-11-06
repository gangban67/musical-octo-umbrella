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

void Reverse(SqList *L)
{
    int i, j, t;
    for (i = 0, j = L->length - 1; i < j; i++, j--) {
        t = L->data[i];
        L->data[i] = L->data[j];
        L->data[j] = t;
    }
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
    int i;

    InitList(&L);
    for (i = 1; i <= 8; i++)
        Insert(&L, i, i);
    Reverse(&L);
    Print(L);
    return 0;
}
