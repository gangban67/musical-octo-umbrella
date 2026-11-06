#include <stdio.h>

#define MAX 10

typedef char ElemType;

typedef struct {
    ElemType data[MAX];
    int front;
    int rear;
} SqQueue;

void InitQueue(SqQueue *q)
{
    q->front = 0;
    q->rear = 0;
}

int QueueEmpty(SqQueue q)
{
    return q.front == q.rear;
}

int EnQueue(SqQueue *q, ElemType e)
{
    if ((q->rear + 1) % MAX == q->front)
        return 0;
    q->data[q->rear] = e;
    q->rear = (q->rear + 1) % MAX;
    return 1;
}

int DeQueue(SqQueue *q, ElemType *e)
{
    if (q->front == q->rear)
        return 0;
    *e = q->data[q->front];
    q->front = (q->front + 1) % MAX;
    return 1;
}

int QueueLength(SqQueue q)
{
    return (q.rear - q.front + MAX) % MAX;
}

void DestroyQueue(SqQueue *q)
{
    q->front = 0;
    q->rear = 0;
}

int main(void)
{
    SqQueue q;
    ElemType e;
    char str1[] = "abc";
    char str2[] = "def";
    int i;

    InitQueue(&q);
    printf("empty=%d\n", QueueEmpty(q));
    for (i = 0; str1[i] != '\0'; i++)
        EnQueue(&q, str1[i]);
    printf("length=%d\n", QueueLength(q));
    DeQueue(&q, &e);
    printf("dequeue=%c\n", e);
    for (i = 0; str2[i] != '\0'; i++)
        EnQueue(&q, str2[i]);
    printf("front=%c\n", q.data[q.front]);
    printf("dequeue:");
    while (DeQueue(&q, &e))
        printf(" %c", e);
    printf("\n");
    printf("empty=%d\n", QueueEmpty(q));
    DestroyQueue(&q);
    return 0;
}
