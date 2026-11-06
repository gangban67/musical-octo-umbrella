#include <stdio.h>
#include <stdlib.h>

typedef char ElemType;

typedef struct DNode {
    ElemType data;
    struct DNode *prior, *next;
} DNode, *DLinkList;

void InitList(DLinkList *h)
{
    *h = (DNode *)malloc(sizeof(DNode));
    (*h)->prior = NULL;
    (*h)->next = NULL;
}

void TailInsert(DLinkList h, ElemType e)
{
    DNode *p = h, *q;
    while (p->next != NULL)
        p = p->next;
    q = (DNode *)malloc(sizeof(DNode));
    q->data = e;
    q->next = NULL;
    q->prior = p;
    p->next = q;
}

int ListInsert(DLinkList h, int i, ElemType e)
{
    DNode *p = h, *q;
    int j = 0;
    if (i < 1)
        return 0;
    while (p != NULL && j < i - 1) {
        p = p->next;
        j++;
    }
    if (p == NULL)
        return 0;
    q = (DNode *)malloc(sizeof(DNode));
    q->data = e;
    q->next = p->next;
    if (p->next != NULL)
        p->next->prior = q;
    q->prior = p;
    p->next = q;
    return 1;
}

int ListDelete(DLinkList h, int i, ElemType *e)
{
    DNode *p = h, *q;
    int j = 0;
    while (p->next != NULL && j < i - 1) {
        p = p->next;
        j++;
    }
    if (p->next == NULL)
        return 0;
    q = p->next;
    *e = q->data;
    p->next = q->next;
    if (q->next != NULL)
        q->next->prior = p;
    free(q);
    return 1;
}

int GetElem(DLinkList h, int i, ElemType *e)
{
    DNode *p = h->next;
    int j = 1;
    if (i < 1)
        return 0;
    while (p != NULL && j < i) {
        p = p->next;
        j++;
    }
    if (p == NULL)
        return 0;
    *e = p->data;
    return 1;
}

int Locate(DLinkList h, ElemType e)
{
    DNode *p = h->next;
    int j = 1;
    while (p != NULL) {
        if (p->data == e)
            return j;
        p = p->next;
        j++;
    }
    return 0;
}

int Length(DLinkList h)
{
    DNode *p = h->next;
    int n = 0;
    while (p != NULL) {
        n++;
        p = p->next;
    }
    return n;
}

int Empty(DLinkList h)
{
    return h->next == NULL;
}

void Print(DLinkList h)
{
    DNode *p = h->next;
    printf("h:");
    while (p != NULL) {
        printf(" %c", p->data);
        p = p->next;
    }
    printf("\n");
}

void DestroyList(DLinkList *h)
{
    DNode *p = (*h)->next, *q;
    while (p != NULL) {
        q = p->next;
        free(p);
        p = q;
    }
    free(*h);
    *h = NULL;
}

int main(void)
{
    DLinkList h;
    ElemType e;
    int i;
    char s[5] = {'a', 'b', 'c', 'd', 'e'};

    InitList(&h);
    for (i = 0; i < 5; i++)
        TailInsert(h, s[i]);
    Print(h);
    printf("length=%d\n", Length(h));
    printf("empty=%d\n", Empty(h));
    GetElem(h, 3, &e);
    printf("h[3]=%c\n", e);
    printf("pos(a)=%d\n", Locate(h, 'a'));
    ListInsert(h, 4, 'f');
    Print(h);
    ListDelete(h, 3, &e);
    Print(h);
    DestroyList(&h);
    printf("destroyed\n");
    return 0;
}
