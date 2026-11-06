#include <stdio.h>
#include <stdlib.h>

typedef char ElemType;

typedef struct qnode {
    ElemType data;
    struct qnode *next;
} QNode;

typedef struct {
    QNode *front;
    QNode *rear;
} LiQueue;

void InitQueue(LiQueue *q)
{
    q->front = q->rear = (QNode *)malloc(sizeof(QNode));
    q->front->next = NULL;
}

int QueueEmpty(LiQueue q)
{
    return q.front == q.rear;
}

void EnQueue(LiQueue *q, ElemType e)
{
    QNode *p = (QNode *)malloc(sizeof(QNode));
    p->data = e;
    p->next = NULL;
    q->rear->next = p;
    q->rear = p;
}

int DeQueue(LiQueue *q, ElemType *e)
{
    QNode *p;
    if (q->front == q->rear)
        return 0;
    p = q->front->next;
    *e = p->data;
    q->front->next = p->next;
    if (q->rear == p)
        q->rear = q->front;
    free(p);
    return 1;
}

void DestroyQueue(LiQueue *q)
{
    QNode *p = q->front, *r;
    while (p != NULL) {
        r = p->next;
        free(p);
        p = r;
    }
    q->front = q->rear = NULL;
}

int main(void)
{
    LiQueue q;
    ElemType e;
    char str1[] = "abc";
    char str2[] = "def";
    int i;

    InitQueue(&q);
    printf("empty=%d\n", QueueEmpty(q));
    for (i = 0; str1[i] != '\0'; i++)
        EnQueue(&q, str1[i]);
    DeQueue(&q, &e);
    printf("dequeue=%c\n", e);
    for (i = 0; str2[i] != '\0'; i++)
        EnQueue(&q, str2[i]);
    printf("front=%c\n", q.front->next->data);
    printf("dequeue:");
    while (DeQueue(&q, &e))
        printf(" %c", e);
    printf("\n");
    printf("empty=%d\n", QueueEmpty(q));
    DestroyQueue(&q);
    return 0;
}
