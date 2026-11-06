#include <stdio.h>

#define MAX 20

int col[MAX];

int Place(int row, int c)
{
    int i;
    for (i = 0; i < row; i++) {
        if (col[i] == c)
            return 0;
        if (col[i] - c == row - i || c - col[i] == row - i)
            return 0;
    }
    return 1;
}

int main(void)
{
    int n, row, count = 0, i;

    scanf("%d", &n);
    row = 0;
    col[0] = -1;

    while (row >= 0) {
        col[row]++;
        while (col[row] < n && !Place(row, col[row]))
            col[row]++;
        if (col[row] < n) {
            if (row == n - 1) {
                count++;
                printf("solution %d:", count);
                for (i = 0; i < n; i++)
                    printf(" %d", col[i] + 1);
                printf("\n");
            } else {
                row++;
                col[row] = -1;
            }
        } else {
            row--;
        }
    }

    printf("total=%d\n", count);
    return 0;
}
