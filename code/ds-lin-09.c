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

Poly AddPoly(Poly a, Poly b)
{
    Poly c = InitPoly();
    Poly r = c, pa = a->next, pb = b->next;
    int sum;

    while (pa != NULL && pb != NULL) {
        if (pa->exp < pb->exp) {
            AddTerm(&r, pa->coef, pa->exp);
            pa = pa->next;
        } else if (pa->exp > pb->exp) {
            AddTerm(&r, pb->coef, pb->exp);
            pb = pb->next;
        } else {
            sum = pa->coef + pb->coef;
            if (sum != 0)
                AddTerm(&r, sum, pa->exp);
            pa = pa->next;
            pb = pb->next;
        }
    }
    while (pa != NULL) {
        AddTerm(&r, pa->coef, pa->exp);
        pa = pa->next;
    }
    while (pb != NULL) {
        AddTerm(&r, pb->coef, pb->exp);
        pb = pb->next;
    }
    return c;
}

int main(void)
{
    int c1[] = {3, 5, 2};
    int e1[] = {0, 2, 5};
    int c2[] = {4, -5, 3, 7};
    int e2[] = {1, 2, 5, 9};
    Poly p1 = Create(c1, e1, 3);
    Poly p2 = Create(c2, e2, 4);

    PrintPoly("P1: ", p1);
    PrintPoly("P2: ", p2);
    PrintPoly("sum: ", AddPoly(p1, p2));
    return 0;
}
