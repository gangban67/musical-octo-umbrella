#include <stdio.h>
#include <stdlib.h>

typedef char ElemType;

typedef struct node {
    ElemType data;
    struct node *next;
} LiNode, *LiStack;

void InitStack(LiStack *s)
{
    *s = NULL;
}

int StackEmpty(LiStack s)
{
    return s == NULL;
}

void Push(LiStack *s, ElemType e)
{
    LiNode *p = (LiNode *)malloc(sizeof(LiNode));
    p->data = e;
    p->next = *s;
    *s = p;
}

int Pop(LiStack *s, ElemType *e)
{
    LiNode *p;
    if (*s == NULL)
        return 0;
    p = *s;
    *e = p->data;
    *s = p->next;
    free(p);
    return 1;
}

int GetTop(LiStack s, ElemType *e)
{
    if (s == NULL)
        return 0;
    *e = s->data;
    return 1;
}

void DestroyStack(LiStack *s)
{
    LiNode *p;
    while (*s != NULL) {
        p = *s;
        *s = p->next;
        free(p);
    }
}

int main(void)
{
    LiStack s;
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
