#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    char data;
    struct node *next;
} LiString;

LiString *StrAssign(const char *t)
{
    LiString *s, *p, *tail;
    int i;

    s = (LiString *)malloc(sizeof(LiString));
    s->data = '\0';
    s->next = NULL;
    tail = s;
    for (i = 0; t[i] != '\0'; i++) {
        p = (LiString *)malloc(sizeof(LiString));
        p->data = t[i];
        p->next = NULL;
        tail->next = p;
        tail = p;
    }
    return s;
}

void Append(LiString *r, LiString **tail, char c)
{
    LiString *p;

    p = (LiString *)malloc(sizeof(LiString));
    p->data = c;
    p->next = NULL;
    (*tail)->next = p;
    *tail = p;
}

int StrLength(LiString *s)
{
    int n = 0;
    LiString *p;

    for (p = s->next; p != NULL; p = p->next)
        n++;
    return n;
}

void DispStr(LiString *s)
{
    LiString *p;

    for (p = s->next; p != NULL; p = p->next)
        printf("%c", p->data);
}

LiString *InsStr(LiString *s, int i, LiString *t)
{
    LiString *r, *tail, *p, *q;
    int k = 1;

    r = (LiString *)malloc(sizeof(LiString));
    r->next = NULL;
    tail = r;
    for (p = s->next; p != NULL && k < i; p = p->next, k++)
        Append(r, &tail, p->data);
    for (q = t->next; q != NULL; q = q->next)
        Append(r, &tail, q->data);
    for (; p != NULL; p = p->next)
        Append(r, &tail, p->data);
    return r;
}

LiString *DelStr(LiString *s, int i, int n)
{
    LiString *r, *tail, *p;
    int k = 1;

    r = (LiString *)malloc(sizeof(LiString));
    r->next = NULL;
    tail = r;
    for (p = s->next; p != NULL && k < i; p = p->next, k++)
        Append(r, &tail, p->data);
    for (k = 0; k < n && p != NULL; k++)
        p = p->next;
    for (; p != NULL; p = p->next)
        Append(r, &tail, p->data);
    return r;
}

LiString *RepStr(LiString *s, int i, int n, LiString *t)
{
    LiString *r, *tail, *p, *q;
    int k = 1;

    r = (LiString *)malloc(sizeof(LiString));
    r->next = NULL;
    tail = r;
    for (p = s->next; p != NULL && k < i; p = p->next, k++)
        Append(r, &tail, p->data);
    for (q = t->next; q != NULL; q = q->next)
        Append(r, &tail, q->data);
    for (k = 0; k < n && p != NULL; k++)
        p = p->next;
    for (; p != NULL; p = p->next)
        Append(r, &tail, p->data);
    return r;
}

LiString *SubStr(LiString *s, int i, int n)
{
    LiString *r, *tail, *p;
    int k = 1;

    r = (LiString *)malloc(sizeof(LiString));
    r->next = NULL;
    tail = r;
    for (p = s->next; p != NULL && k < i; p = p->next, k++)
        ;
    for (k = 0; k < n && p != NULL; p = p->next, k++)
        Append(r, &tail, p->data);
    return r;
}

LiString *Concat(LiString *s, LiString *t)
{
    LiString *r, *tail, *p;

    r = (LiString *)malloc(sizeof(LiString));
    r->next = NULL;
    tail = r;
    for (p = s->next; p != NULL; p = p->next)
        Append(r, &tail, p->data);
    for (p = t->next; p != NULL; p = p->next)
        Append(r, &tail, p->data);
    return r;
}

int main(void)
{
    LiString *s, *s1, *s2, *s3, *s4;

    s = StrAssign("abcdefghijklmn");
    s1 = StrAssign("xyz");

    printf("s=");
    DispStr(s);
    printf("\n");
    printf("len(s)=%d\n", StrLength(s));

    s2 = InsStr(s, 9, s1);
    printf("s2=");
    DispStr(s2);
    printf("\n");
    printf("len(s2)=%d\n", StrLength(s2));

    s2 = DelStr(s, 2, 5);
    printf("s2=");
    DispStr(s2);
    printf("\n");
    printf("len(s2)=%d\n", StrLength(s2));

    s2 = RepStr(s, 2, 5, s1);
    printf("s2=");
    DispStr(s2);
    printf("\n");
    printf("len(s2)=%d\n", StrLength(s2));

    s3 = SubStr(s, 2, 10);
    printf("s3=");
    DispStr(s3);
    printf("\n");
    printf("len(s3)=%d\n", StrLength(s3));

    s4 = Concat(s1, s2);
    printf("s4=");
    DispStr(s4);
    printf("\n");
    printf("len(s4)=%d\n", StrLength(s4));

    return 0;
}
