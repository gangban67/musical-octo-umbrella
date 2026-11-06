#include <stdio.h>
#include <stdlib.h>

typedef struct PNode {
    int coef;
    int exp;
    struct PNode *next;
} PNode, *Poly;

Poly InitPoly(void)
{
    Poly head = (Poly)malloc(sizeof(PNode));
    head->next = NULL;
    return head;
}

void AddTerm(Poly *tail, int coef, int exp)
{
    Poly s = (Poly)malloc(sizeof(PNode));
    s->coef = coef;
    s->exp = exp;
    s->next = NULL;
    (*tail)->next = s;
    *tail = s;
}

Poly Create(int c[], int e[], int n)
{
    Poly head = InitPoly();
    Poly r = head;
    int i;

    for (i = 0; i < n; i++)
        AddTerm(&r, c[i], e[i]);
    return head;
}

void InsertTerm(Poly head, int coef, int exp)
{
    Poly pre = head, p = head->next, s;

    while (p != NULL && p->exp < exp) {
        pre = p;
        p = p->next;
    }
    if (p != NULL && p->exp == exp) {
        p->coef += coef;
        if (p->coef == 0) {
            pre->next = p->next;
            free(p);
        }
        return;
    }
    s = (Poly)malloc(sizeof(PNode));
    s->coef = coef;
    s->exp = exp;
    s->next = p;
    pre->next = s;
}

Poly MulPoly(Poly a, Poly b)
{
    Poly c = InitPoly();
    Poly pa, pb;

    for (pa = a->next; pa != NULL; pa = pa->next)
        for (pb = b->next; pb != NULL; pb = pb->next)
            InsertTerm(c, pa->coef * pb->coef, pa->exp + pb->exp);
    return c;
}

void PrintPoly(char *tag, Poly head)
{
    Poly p = head->next;
    int first = 1, c;

    printf("%s", tag);
    if (p == NULL) {
        printf("0\n");
        return;
    }
    while (p != NULL) {
        if (p->coef > 0) {
            if (!first)
                printf("+");
        } else {
            printf("-");
        }
        c = p->coef < 0 ? -p->coef : p->coef;
        if (p->exp == 0)
            printf("%d", c);
        else if (p->exp == 1)
            printf("%dx", c);
        else
            printf("%dx^%d", c, p->exp);
        first = 0;
        p = p->next;
    }
    printf("\n");
}

int main(void)
{
    int c1[] = {1, 2};
    int e1[] = {0, 1};
    int c2[] = {3, 4, 5};
    int e2[] = {0, 1, 2};
    Poly p1 = Create(c1, e1, 2);
    Poly p2 = Create(c2, e2, 3);

    PrintPoly("P1: ", p1);
    PrintPoly("P2: ", p2);
    PrintPoly("mul: ", MulPoly(p1, p2));
    return 0;
}
