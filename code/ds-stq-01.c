#include <stdio.h>

#define MAX 20

typedef char ElemType;

typedef struct {
    ElemType data[MAX];
    int top;
} SqStack;

void InitStack(SqStack *s)
{
    s->top = -1;
}

int StackEmpty(SqStack s)
{
    return s.top == -1;
}

int Push(SqStack *s, ElemType e)
{
    if (s->top == MAX - 1)
        return 0;
    s->top++;
    s->data[s->top] = e;
    return 1;
}

int Pop(SqStack *s, ElemType *e)
{
    if (s->top == -1)
        return 0;
    *e = s->data[s->top];
    s->top--;
    return 1;
}

int GetTop(SqStack s, ElemType *e)
{
    if (s.top == -1)
        return 0;
    *e = s.data[s.top];
    return 1;
}

void DestroyStack(SqStack *s)
{
    s->top = -1;
}

int main(void)
{
    SqStack s;
    ElemType e;
    char str[] = "abcde";
    int i;

    InitStack(&s);
    printf("empty=%d\n", StackEmpty(s));
    for (i = 0; str[i] != '\0'; i++)
        Push(&s, str[i]);
    GetTop(s, &e);
    printf("top=%c\n", e);
    printf("empty=%d\n", StackEmpty(s));
    printf("pop:");
    while (Pop(&s, &e))
        printf(" %c", e);
    printf("\n");
    printf("empty=%d\n", StackEmpty(s));
    DestroyStack(&s);
    return 0;
}
