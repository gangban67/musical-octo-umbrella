#include <stdio.h>
#include <stdlib.h>

typedef struct LNode {
    int data;
    struct LNode *next;
} LNode, *LinkList;

LinkList Create(int a[], int n)
{
    LinkList head = (LinkList)malloc(sizeof(LNode));
    LNode *r = head, *s;
    int i;

    head->next = NULL;
    for (i = 0; i < n; i++) {
        s = (LNode *)malloc(sizeof(LNode));
        s->data = a[i];
        s->next = NULL;
        r->next = s;
        r = s;
    }
    return head;
}

void Print(char *tag, LinkList head)
{
    LNode *p = head->next;

    printf("%s", tag);
    while (p != NULL) {
        printf("%d%s", p->data, p->next == NULL ? "\n" : " ");
        p = p->next;
    }
}

LinkList Merge(LinkList a, LinkList b)
{
    LNode *pa = a->next, *pb = b->next;
    LNode *tail = a, *na, *nb;

    while (pa != NULL && pb != NULL) {
        na = pa->next;
        nb = pb->next;
        tail->next = pa;
        tail = pa;
        tail->next = pb;
        tail = pb;
        pa = na;
        pb = nb;
    }
    if (pa != NULL)
        tail->next = pa;
    else
        tail->next = pb;
    free(b);
    return a;
}

void Demo(int x[], int n, int y[], int m)
{
    LinkList a = Create(x, n);
    LinkList b = Create(y, m);
    LinkList c;

    Print("L1: ", a);
    Print("L2: ", b);
    c = Merge(a, b);
    Print("L3: ", c);
}

int main(void)
{
    int x1[] = {1, 3, 5, 7, 9};
    int y1[] = {2, 4, 6};
    int x2[] = {1, 3, 5};
    int y2[] = {2, 4, 6, 8, 10};

    Demo(x1, 5, y1, 3);
    Demo(x2, 3, y2, 5);
    return 0;
}
