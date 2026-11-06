#include <stdio.h>
#include <string.h>

#define N 3
#define RATE 2

typedef struct {
    char plate[10];
    int arrive;
} Car;

Car lot[N];
int lotn = 0;

Car waitq[50];
int wf = 0, wr = 0;

void Park(Car c)
{
    lot[lotn] = c;
    lotn++;
    printf("park %s %d pos=%d\n", c.plate, c.arrive, lotn);
}

int FindCar(const char *plate)
{
    int i;
    for (i = 0; i < lotn; i++)
        if (strcmp(lot[i].plate, plate) == 0)
            return i;
    return -1;
}

int main(void)
{
    char op[10], plate[10];
    int time, j, k, idx, stay;
    Car c, temp[N], t;
    int tn;

    while (scanf("%s", op) == 1) {
        if (strcmp(op, "A") == 0) {
            scanf("%s %d", plate, &time);
            strcpy(c.plate, plate);
            c.arrive = time;
            if (lotn < N) {
                Park(c);
            } else {
                waitq[wr++] = c;
                printf("wait %s %d\n", c.plate, c.arrive);
            }
        } else if (strcmp(op, "D") == 0) {
            scanf("%s %d", plate, &time);
            idx = FindCar(plate);
            if (idx < 0) {
                printf("notfound %s\n", plate);
                continue;
            }
            tn = 0;
            for (j = lotn - 1; j > idx; j--)
                temp[tn++] = lot[j];
            c = lot[idx];
            lotn = idx;
            stay = time - c.arrive;
            printf("depart %s %d pos=%d stay=%d fee=%d\n",
                   c.plate, time, idx + 1, stay, stay * RATE);
            for (k = tn - 1; k >= 0; k--) {
                lot[lotn++] = temp[k];
                printf("move %s pos=%d\n", temp[k].plate, lotn);
            }
            if (wf < wr && lotn < N) {
                t = waitq[wf++];
                Park(t);
            }
        } else if (strcmp(op, "E") == 0) {
            printf("exit\n");
            break;
        }
    }
    return 0;
}
