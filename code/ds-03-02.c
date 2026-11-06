#include <stdio.h>
#include <stdlib.h>

typedef struct LNode {
    int data;
    struct LNode *next;
} LNode, *LinkList;

void InitList(LinkList *head)
{
    *head = (LNode *)malloc(sizeof(LNode));
    (*head)->next = NULL;
}

void TailInsert(LinkList head, int e)
{
    LNode *s = (LNode *)malloc(sizeof(LNode));
    LNode *r = head;
    s->data = e;
    s->next = NULL;
    while (r->next != NULL)
        r = r->next;
    r->next = s;
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

void Print(LinkList head, const char *tag)
{
    LNode *p = head->next;
    printf("%s", tag);
    while (p != NULL) {
        printf("%d%s", p->data, p->next == NULL ? "\n" : " ");
        p = p->next;
    }
}

int main(void)
{
    LinkList head;
    int n, i, x;

    InitList(&head);
    scanf("%d", &n);
    for (i = 0; i < n; i++) {
        scanf("%d", &x);
        TailInsert(head, x);
    }
    Print(head, "before: ");
    Reverse(head);
    Print(head, "after: ");
    return 0;
}
