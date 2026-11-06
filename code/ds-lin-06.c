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

void Partition(LinkList head, int x)
{
    LNode *p = head->next, *q;
    LNode *less = NULL, *lessTail = NULL, *ge = NULL, *geTail = NULL;

    while (p != NULL) {
        q = p->next;
        p->next = NULL;
        if (p->data < x) {
            if (less == NULL) {
                less = p;
                lessTail = p;
            } else {
                lessTail->next = p;
                lessTail = p;
            }
        } else {
            if (ge == NULL) {
                ge = p;
                geTail = p;
            } else {
                geTail->next = p;
                geTail = p;
            }
        }
        p = q;
    }
    if (less == NULL) {
        head->next = ge;
    } else {
        lessTail->next = ge;
        head->next = less;
    }
}

int main(void)
{
    int a[] = {5, 3, 8, 1, 9, 2, 7, 4, 6};
    LinkList head = Create(a, 9);

    Print("before: ", head);
    Partition(head, a[0]);
    Print("after: ", head);
    return 0;
}
