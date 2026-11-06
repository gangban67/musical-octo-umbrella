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

int main(void)
{
    SqString s;
    int i, j, k, maxlen = 0, pos = 0;

    StrAssign(&s, "abcabcabc");

    for (i = 0; i < s.length; i++) {
        for (j = i + 1; j < s.length; j++) {
            k = 0;
            while (i + k < s.length && j + k < s.length &&
                   s.data[i + k] == s.data[j + k])
                k++;
            if (k > maxlen) {
                maxlen = k;
                pos = i;
            }
        }
    }

    printf("s=");
    DispStr(&s);
    printf("\n");
    printf("index=%d\n", pos);
    printf("len=%d\n", maxlen);
    printf("sub=");
    for (k = 0; k < maxlen; k++)
        printf("%c", s.data[pos + k]);
    printf("\n");
    return 0;
}
