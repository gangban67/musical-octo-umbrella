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

int KMP(const SqString *s, const SqString *t, const int next[], int from)
{
    int i = from, j = 1;
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
    int next[MAX], pos, count = 0, k;

    StrAssign(&s, "aaaaa");
    StrAssign(&t, "aa");
    GetNext(&t, next);

    printf("s=");
    DispStr(&s);
    printf("\n");
    printf("t=");
    DispStr(&t);
    printf("\n");

    pos = KMP(&s, &t, next, 1);
    while (pos > 0) {
        count++;
        printf("match %d: start=%d text=", count, pos);
        for (k = 0; k < t.length; k++)
            printf("%c", s.data[pos - 1 + k]);
        printf("\n");
        pos = KMP(&s, &t, next, pos + 1);
    }
    printf("count=%d\n", count);
    return 0;
}
