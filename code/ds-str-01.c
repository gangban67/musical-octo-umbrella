#include <stdio.h>

#define MAX 100

typedef struct {
    char data[MAX];
    int length;
} SqString;

void StrAssign(SqString *s, const char *t)
{
    int i;
    for (i = 0; t[i] != '\0'; i++)
        s->data[i] = t[i];
    s->length = i;
}

void DispStr(const SqString *s)
{
    int i;
    for (i = 0; i < s->length; i++)
        printf("%c", s->data[i]);
}

void InsStr(const SqString *s, int i, const SqString *t, SqString *r)
{
    int k;
    r->length = 0;
    if (i < 1 || i > s->length + 1)
        return;
    for (k = 0; k < i - 1; k++)
        r->data[r->length++] = s->data[k];
    for (k = 0; k < t->length; k++)
        r->data[r->length++] = t->data[k];
    for (k = i - 1; k < s->length; k++)
        r->data[r->length++] = s->data[k];
}

void DelStr(const SqString *s, int i, int n, SqString *r)
{
    int k;
    r->length = 0;
    if (i < 1 || i + n - 1 > s->length)
        return;
    for (k = 0; k < i - 1; k++)
        r->data[r->length++] = s->data[k];
    for (k = i - 1 + n; k < s->length; k++)
        r->data[r->length++] = s->data[k];
}

void RepStr(const SqString *s, int i, int n, const SqString *t, SqString *r)
{
    int k;
    r->length = 0;
    if (i < 1 || i + n - 1 > s->length)
        return;
    for (k = 0; k < i - 1; k++)
        r->data[r->length++] = s->data[k];
    for (k = 0; k < t->length; k++)
        r->data[r->length++] = t->data[k];
    for (k = i - 1 + n; k < s->length; k++)
        r->data[r->length++] = s->data[k];
}

void SubStr(const SqString *s, int i, int n, SqString *r)
{
    int k;
    r->length = 0;
    if (i < 1 || i + n - 1 > s->length)
        return;
    for (k = 0; k < n; k++)
        r->data[r->length++] = s->data[i - 1 + k];
}

void Concat(const SqString *s, const SqString *t, SqString *r)
{
    int k;
    r->length = 0;
    for (k = 0; k < s->length; k++)
        r->data[r->length++] = s->data[k];
    for (k = 0; k < t->length; k++)
        r->data[r->length++] = t->data[k];
}

int main(void)
{
    SqString s, s1, s2, s3, s4;

    StrAssign(&s, "abcdefghijklmn");
    StrAssign(&s1, "xyz");

    printf("s=");
    DispStr(&s);
    printf("\n");
    printf("len(s)=%d\n", s.length);

    InsStr(&s, 9, &s1, &s2);
    printf("s2=");
    DispStr(&s2);
    printf("\n");
    printf("len(s2)=%d\n", s2.length);

    DelStr(&s, 2, 5, &s2);
    printf("s2=");
    DispStr(&s2);
    printf("\n");
    printf("len(s2)=%d\n", s2.length);

    RepStr(&s, 2, 5, &s1, &s2);
    printf("s2=");
    DispStr(&s2);
    printf("\n");
    printf("len(s2)=%d\n", s2.length);

    SubStr(&s, 2, 10, &s3);
    printf("s3=");
    DispStr(&s3);
    printf("\n");
    printf("len(s3)=%d\n", s3.length);

    Concat(&s1, &s2, &s4);
    printf("s4=");
    DispStr(&s4);
    printf("\n");
    printf("len(s4)=%d\n", s4.length);

    return 0;
}
