#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

void HeadInsert(LinkList head, int v)
{
    LNode *s = (LNode *)malloc(sizeof(LNode));
    s->data = v;
    s->next = head->next;
    head->next = s;
}

LinkList FromString(char *s)
{
    LinkList head = InitList();
    int i, n = (int)strlen(s);

    for (i = 0; i < n; i++)
        HeadInsert(head, s[i] - '0');
    return head;
}

LinkList Add(LinkList a, LinkList b)
{
    LinkList c = InitList();
    LNode *r = c, *pa = a->next, *pb = b->next, *s;
    int carry = 0, v;

    while (pa != NULL || pb != NULL || carry != 0) {
        v = carry;
        if (pa != NULL) {
            v += pa->data;
            pa = pa->next;
        }
        if (pb != NULL) {
            v += pb->data;
            pb = pb->next;
        }
        s = (LNode *)malloc(sizeof(LNode));
        s->data = v % 10;
        s->next = NULL;
        r->next = s;
        r = s;
        carry = v / 10;
    }
    return c;
}

void Reverse(LinkList head)
{
    LNode *pre = NULL, *p = head->next, *q;

    while (p != NULL) {
        q = p->next;
        p->next = pre;
        pre = p;
        p = q;
    }
    head->next = pre;
}

void PrintNum(char *tag, LinkList head)
{
    LNode *p;

    printf("%s", tag);
    for (p = head->next; p != NULL; p = p->next)
        printf("%d", p->data);
    printf("\n");
}

int Length(LinkList head)
{
    int n = 0;
    LNode *p = head->next;

    while (p != NULL) {
        n++;
        p = p->next;
    }
    return n;
}

int Middle(LinkList head)
{
    LNode *p = head->next;
    int n = Length(head), pos = (n + 1) / 2, i;

    for (i = 1; i < pos; i++)
        p = p->next;
    return p->data;
}

int main(void)
{
    char s1[128], s2[128];
    LinkList a, b, c;

    scanf("%127s", s1);
    scanf("%127s", s2);
    a = FromString(s1);
    b = FromString(s2);
    c = Add(a, b);
    Reverse(c);
    Reverse(a);
    Reverse(b);

    PrintNum("A: ", a);
    PrintNum("B: ", b);
    PrintNum("sum: ", c);
    printf("len: %d\n", Length(c));
    printf("middle: %d\n", Middle(c));
    return 0;
}
