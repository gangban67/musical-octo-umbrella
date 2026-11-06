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

int LeafCount(BiTree t)
{
    if (t == NULL)
        return 0;
    if (t->lchild == NULL && t->rchild == NULL)
        return 1;
    return LeafCount(t->lchild) + LeafCount(t->rchild);
}

int main(void)
{
    BiTree root = CreateTree();
    printf("leaves=%d\n", LeafCount(root));
    return 0;
}
