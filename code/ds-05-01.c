#include <stdio.h>
#include <stdlib.h>

typedef struct BiTNode {
    char data;
    struct BiTNode *lchild, *rchild;
} BiTNode, *BiTree;

char nextChar(void)
{
    char c;
    while ((c = getchar()) != EOF && (c == ' ' || c == '\n' || c == '\r'))
        ;
    return c;
}

BiTree CreateTree(void)
{
    char c = nextChar();
    BiTree t;
    if (c == '#' || c == EOF)
        return NULL;
    t = (BiTNode *)malloc(sizeof(BiTNode));
    t->data = c;
    t->lchild = CreateTree();
    t->rchild = CreateTree();
    return t;
}

void PreOrder(BiTree t)
{
    if (t == NULL)
        return;
    printf(" %c", t->data);
    PreOrder(t->lchild);
    PreOrder(t->rchild);
}

void InOrder(BiTree t)
{
    if (t == NULL)
        return;
    InOrder(t->lchild);
    printf(" %c", t->data);
    InOrder(t->rchild);
}

void PostOrder(BiTree t)
{
    if (t == NULL)
        return;
    PostOrder(t->lchild);
    PostOrder(t->rchild);
    printf(" %c", t->data);
}

int main(void)
{
    BiTree root = CreateTree();

    printf("pre:");
    PreOrder(root);
    printf("\n");
    printf("in:");
    InOrder(root);
    printf("\n");
    printf("post:");
    PostOrder(root);
    printf("\n");
    return 0;
}
