#include <stdio.h>

#define MAX 30

typedef struct {
    char data[MAX];
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

void Push(SqStack *s, char e)
{
    s->top++;
    s->data[s->top] = e;
}

char Pop(SqStack *s)
{
    return s->data[s->top--];
}

char GetTop(SqStack s)
{
    return s.data[s.top];
}

void PrintStack(SqStack s)
{
    int i;
    printf("[");
    for (i = s.top; i >= 0; i--)
        printf("%c%s", s.data[i], i > 0 ? " " : "");
    printf("]");
}

int main(void)
{
    SqStack s, t;
    char str[100];
    char x, y;
    int i, step = 0;

    scanf("%s", str);
    InitStack(&s);
    InitStack(&t);
    for (i = 0; str[i] != '\0'; i++)
        Push(&s, str[i]);

    printf("before: s=");
    PrintStack(s);
    printf(" t=");
    PrintStack(t);
    printf("\n");

    while (!StackEmpty(s)) {
        x = Pop(&s);
        while (!StackEmpty(t) && GetTop(t) > x) {
            y = Pop(&t);
            Push(&s, y);
        }
        Push(&t, x);
        step++;
        printf("step %d: s=", step);
        PrintStack(s);
        printf(" t=");
        PrintStack(t);
        printf("\n");
    }

    while (!StackEmpty(t)) {
        y = Pop(&t);
        Push(&s, y);
    }
    printf("sorted: ");
    PrintStack(s);
    printf("\n");
    return 0;
}
