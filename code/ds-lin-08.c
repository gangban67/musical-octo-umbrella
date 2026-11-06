#include <stdio.h>
#include <stdlib.h>

typedef struct LNode {
    int data;
    struct LNode *next;
} LNode, *LinkList;

LinkList InitList(void)
{
    LinkList head = (LinkList)malloc(sizeof(LNode));
    head->next = NULL;
    return head;
}

void Add(LNode **tail, int v)
{
    LNode *s = (LNode *)malloc(sizeof(LNode));
    s->data = v;
    s->next = NULL;
    (*tail)->next = s;
    *tail = s;
}

LinkList Create(int a[], int n)
{
    LinkList head = InitList();
    LNode *r = head;
    int i;

    for (i = 0; i < n; i++)
        Add(&r, a[i]);
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

LinkList Union(LinkList a, LinkList b)
{
    LinkList c = InitList();
    LNode *r = c, *pa = a->next, *pb = b->next;

    while (pa != NULL && pb != NULL) {
        if (pa->data < pb->data) {
            Add(&r, pa->data);
            pa = pa->next;
        } else if (pa->data > pb->data) {
            Add(&r, pb->data);
            pb = pb->next;
        } else {
            Add(&r, pa->data);
            pa = pa->next;
            pb = pb->next;
        }
    }
    while (pa != NULL) {
        Add(&r, pa->data);
        pa = pa->next;
    }
    while (pb != NULL) {
        Add(&r, pb->data);
        pb = pb->next;
    }
    return c;
}

LinkList Intersect(LinkList a, LinkList b)
{
    LinkList c = InitList();
    LNode *r = c, *pa = a->next, *pb = b->next;

    while (pa != NULL && pb != NULL) {
        if (pa->data < pb->data) {
            pa = pa->next;
        } else if (pa->data > pb->data) {
            pb = pb->next;
        } else {
            Add(&r, pa->data);
            pa = pa->next;
            pb = pb->next;
        }
    }
    return c;
}

LinkList Difference(LinkList a, LinkList b)
{
    LinkList c = InitList();
    LNode *r = c, *pa = a->next, *pb = b->next;

    while (pa != NULL && pb != NULL) {
        if (pa->data < pb->data) {
            Add(&r, pa->data);
            pa = pa->next;
        } else if (pa->data > pb->data) {
            pb = pb->next;
        } else {
            pa = pa->next;
            pb = pb->next;
        }
    }
    while (pa != NULL) {
        Add(&r, pa->data);
        pa = pa->next;
    }
    return c;
}

int main(void)
{
    int sa[] = {1, 3, 5, 7, 9};
    int sb[] = {2, 3, 6, 9, 10};
    LinkList a = Create(sa, 5);
    LinkList b = Create(sb, 5);

    Print("A: ", a);
    Print("B: ", b);
    Print("union: ", Union(a, b));
    Print("inter: ", Intersect(a, b));
    Print("diff: ", Difference(a, b));
    return 0;
}
