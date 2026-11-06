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

void HeadInsert(LinkList head, int e)
{
    LNode *s = (LNode *)malloc(sizeof(LNode));
    s->data = e;
    s->next = head->next;
    head->next = s;
}

void Print(LinkList head)
{
    LNode *p = head->next;
    while (p != NULL) {
        printf("%d%s", p->data, p->next == NULL ? "\n" : " ");
        p = p->next;
    }
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

int main(void)
{
    LinkList head;
    int i;

    InitList(&head);
    for (i = 1; i <= 5; i++)
        HeadInsert(head, i);
    Print(head);
    printf("length=%d\n", Length(head));
    return 0;
}
