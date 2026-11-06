#include <stdio.h>
#include <string.h>

#define MAX 50

int main(void)
{
    int q[MAX];
    int front = 0, rear = 0;
    int id, i;
    char op[10];

    while (scanf("%s", op) == 1) {
        if (strcmp(op, "1") == 0) {
            scanf("%d", &id);
            if (rear < MAX) {
                q[rear++] = id;
                printf("enqueue %d\n", id);
            } else {
                printf("full %d\n", id);
            }
        } else if (strcmp(op, "2") == 0) {
            if (front == rear)
                printf("no patient\n");
            else
                printf("treat %d\n", q[front++]);
        } else if (strcmp(op, "3") == 0) {
            if (front == rear) {
                printf("waiting: none\n");
            } else {
                printf("waiting:");
                for (i = front; i < rear; i++)
                    printf(" %d", q[i]);
                printf("\n");
            }
        } else if (strcmp(op, "4") == 0) {
            if (front == rear) {
                printf("treat all: none\n");
            } else {
                printf("treat all:");
                for (i = front; i < rear; i++)
                    printf(" %d", q[i]);
                printf("\n");
            }
            break;
        } else if (strcmp(op, "5") == 0) {
            printf("bye\n");
            break;
        }
    }
    return 0;
}
