#include <stdio.h>
#include <stdlib.h>

typedef char ElemType;

typedef struct LNode {
    ElemType data;
    struct LNode *next;
} LNode, *LinkList;

void InitList(LinkList *h)
{
    *h = (LNode *)malloc(sizeof(LNode));
    (*h)->next = *h;
}

void TailInsert(LinkList h, ElemType e)
{
    LNode *p = h, *q;
    while (p->next != h)
        p = p->next;
    q = (LNode *)malloc(sizeof(LNode));
    q->data = e;
    q->next = h;
    p->next = q;
}

int ListInsert(LinkList h, int i, ElemType e)
{
    LNode *p = h, *q;
    int j = 0;
    if (i < 1)
        return 0;
    while (p->next != h && j < i - 1) {
        p = p->next;
        j++;
    }
    if (j < i - 1)
        return 0;
    q = (LNode *)malloc(sizeof(LNode));
    q->data = e;
    q->next = p->next;
    p->next = q;
    return 1;
}

int ListDelete(LinkList h, int i, ElemType *e)
{
    LNode *p = h, *q;
    int j = 0;
    while (p->next != h && j < i - 1) {
        p = p->next;
        j++;
    }
    if (p->next == h)
        return 0;
    q = p->next;
    *e = q->data;
    p->next = q->next;
    free(q);
    return 1;
}

int GetElem(LinkList h, int i, ElemType *e)
{
    LNode *p = h->next;
    int j = 1;
    if (i < 1)
        return 0;
    while (p != h && j < i) {
        p = p->next;
        j++;
    }
    if (p == h)
        return 0;
    *e = p->data;
    return 1;
}

int Locate(LinkList h, ElemType e)
{
    LNode *p = h->next;
    int j = 1;
    while (p != h) {
        if (p->data == e)
            return j;
        p = p->next;
        j++;
    }
    return 0;
}

int Length(LinkList h)
{
    LNode *p = h->next;
    int n = 0;
    while (p != h) {
        n++;
        p = p->next;
    }
    return n;
}

int Empty(LinkList h)
{
    return h->next == h;
}

void Print(LinkList h)
{
    LNode *p = h->next;
    printf("h:");
    while (p != h) {
        printf(" %c", p->data);
        p = p->next;
    }
    printf("\n");
}

void DestroyList(LinkList *h)
{
    LNode *p = (*h)->next, *q;
    while (p != *h) {
        q = p->next;
        free(p);
        p = q;
    }
    free(*h);
    *h = NULL;
}

int main(void)
{
    LinkList h;
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
