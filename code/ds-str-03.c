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

void DispArray(int a[], int n)
{
    int i;
    for (i = 1; i <= n; i++)
        printf("%d%s", a[i], i == n ? "\n" : " ");
}

int BF(const SqString *s, const SqString *t)
{
    int i = 0, j = 0;
    while (i < s->length && j < t->length) {
        if (s->data[i] == t->data[j]) {
            i++;
            j++;
        } else {
            i = i - j + 1;
            j = 0;
        }
    }
    if (j >= t->length)
        return i - t->length + 1;
    return 0;
}

void GetNext(const SqString *t, int next[])
{
    int j = 1, k = 0;
    next[1] = 0;
    while (j < t->length) {
        if (k == 0 || t->data[j - 1] == t->data[k - 1]) {
            j++;
            k++;
            next[j] = k;
        } else {
            k = next[k];
        }
    }
}

void GetNextval(const SqString *t, const int next[], int nextval[])
{
    int j;
    nextval[1] = 0;
    for (j = 2; j <= t->length; j++) {
        if (t->data[j - 1] == t->data[next[j] - 1])
            nextval[j] = nextval[next[j]];
        else
            nextval[j] = next[j];
    }
}

int KMP(const SqString *s, const SqString *t, const int next[])
{
    int i = 1, j = 1;
    while (i <= s->length && j <= t->length) {
        if (j == 0 || s->data[i - 1] == t->data[j - 1]) {
            i++;
            j++;
        } else {
            j = next[j];
        }
    }
    if (j > t->length)
        return i - t->length;
    return 0;
}

int main(void)
{
    SqString s, t;
    int next[MAX], nextval[MAX];

    StrAssign(&s, "abcabcdabcdeabcdefabcdefg");
    StrAssign(&t, "abcdeabcdefab");

    printf("s=");
    DispStr(&s);
    printf("\n");
    printf("len(s)=%d\n", s.length);
    printf("t=");
    DispStr(&t);
    printf("\n");
    printf("len(t)=%d\n", t.length);

    printf("BF=%d\n", BF(&s, &t));

    GetNext(&t, next);
    printf("next=");
    DispArray(next, t.length);

    GetNextval(&t, next, nextval);
    printf("nextval=");
    DispArray(nextval, t.length);

    printf("KMP=%d\n", KMP(&s, &t, next));
    printf("KMPval=%d\n", KMP(&s, &t, nextval));

    return 0;
}
