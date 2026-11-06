#include <stdio.h>

#define N 6

int maze[N][N] = {
    {0, 0, 1, 0, 0, 0},
    {0, 0, 0, 1, 0, 1},
    {1, 0, 1, 0, 0, 0},
    {0, 0, 1, 0, 1, 0},
    {0, 1, 1, 0, 1, 0},
    {0, 0, 0, 0, 0, 0}
};

int visited[N][N];

int dx[4] = {0, 1, 0, -1};
int dy[4] = {1, 0, -1, 0};

typedef struct {
    int r;
    int c;
    int d;
} Box;

Box stack[N * N];

int main(void)
{
    int top = 0, count = 0, best = 0, i;
    int found, d, nr, nc, len;

    stack[0].r = 0;
    stack[0].c = 0;
    stack[0].d = -1;
    visited[0][0] = 1;

    while (top >= 0) {
        if (stack[top].r == N - 1 && stack[top].c == N - 1) {
            len = top + 1;
            count++;
            printf("path %d len=%d:", count, len);
            for (i = 0; i <= top; i++)
                printf(" (%d,%d)", stack[i].r, stack[i].c);
            printf("\n");
            if (best == 0 || len < best)
                best = len;
            visited[N - 1][N - 1] = 0;
            top--;
            continue;
        }
        found = 0;
        for (d = stack[top].d + 1; d < 4; d++) {
            nr = stack[top].r + dx[d];
            nc = stack[top].c + dy[d];
            if (nr >= 0 && nr < N && nc >= 0 && nc < N &&
                maze[nr][nc] == 0 && visited[nr][nc] == 0) {
                stack[top].d = d;
                top++;
                stack[top].r = nr;
                stack[top].c = nc;
                stack[top].d = -1;
                visited[nr][nc] = 1;
                found = 1;
                break;
            }
        }
        if (!found) {
            visited[stack[top].r][stack[top].c] = 0;
            top--;
        }
    }

    printf("count=%d\n", count);
    printf("shortest=%d\n", best);
    return 0;
}
