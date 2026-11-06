#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Emp {
    int no;
    char name[20];
    int depno;
    double salary;
    struct Emp *next;
} Emp, *EmpList;

EmpList InitList(void)
{
    EmpList head = (EmpList)malloc(sizeof(Emp));
    head->next = NULL;
    return head;
}

Emp *NewEmp(int no, char *name, int depno, double salary)
{
    Emp *e = (Emp *)malloc(sizeof(Emp));
    e->no = no;
    strcpy(e->name, name);
    e->depno = depno;
    e->salary = salary;
    e->next = NULL;
    return e;
}

void Append(EmpList head, Emp *e)
{
    Emp *p = head;
    while (p->next != NULL)
        p = p->next;
    p->next = e;
}

void Show(char *tag, EmpList head)
{
    Emp *p;

    printf("%s\n", tag);
    for (p = head->next; p != NULL; p = p->next)
        printf("%d %s %d %.2f\n", p->no, p->name, p->depno, p->salary);
}

double KeyOf(Emp *e, int mode)
{
    if (mode == 1)
        return e->no;
    if (mode == 2)
        return e->depno;
    return e->salary;
}

void Sort(EmpList head, int mode)
{
    Emp *a[64], *p, *cur;
    int n = 0, i, j;

    for (p = head->next; p != NULL; p = p->next)
        a[n++] = p;
    for (i = 1; i < n; i++) {
        cur = a[i];
        j = i - 1;
        while (j >= 0 && KeyOf(a[j], mode) > KeyOf(cur, mode)) {
            a[j + 1] = a[j];
            j--;
        }
        a[j + 1] = cur;
    }
    if (n > 0)
        head->next = a[0];
    for (i = 0; i < n - 1; i++)
        a[i]->next = a[i + 1];
    if (n > 0)
        a[n - 1]->next = NULL;
}

void DeleteNo(EmpList head, int no)
{
    Emp *pre = head, *p = head->next;

    while (p != NULL && p->no != no) {
        pre = p;
        p = p->next;
    }
    if (p != NULL) {
        pre->next = p->next;
        free(p);
    }
}

void DeleteAll(EmpList head)
{
    Emp *p = head->next, *q;

    while (p != NULL) {
        q = p->next;
        free(p);
        p = q;
    }
    head->next = NULL;
}

int main(void)
{
    EmpList h = InitList();
    Emp *p;
    int saved = 0;

    Append(h, NewEmp(1003, "Wang", 2, 3500.0));
    Append(h, NewEmp(1001, "Li", 1, 4200.0));
    Append(h, NewEmp(1004, "Zhao", 2, 2800.0));
    Append(h, NewEmp(1002, "Sun", 3, 5600.0));
    Append(h, NewEmp(1005, "Qian", 1, 3100.0));
    printf("load: 5 records\n");

    Append(h, NewEmp(1006, "Zhou", 3, 3900.0));
    printf("insert: 1006 Zhou 3 3900.00\n");

    Show("show all:", h);

    Sort(h, 1);
    Show("sort by no:", h);

    Sort(h, 2);
    Show("sort by depno:", h);

    Sort(h, 3);
    Show("sort by salary:", h);

    DeleteNo(h, 1004);
    Show("delete no=1004:", h);

    printf("save to emp.dat:\n");
    for (p = h->next; p != NULL; p = p->next) {
        printf("%d %s %d %.2f\n", p->no, p->name, p->depno, p->salary);
        saved++;
    }
    printf("saved %d records\n", saved);

    DeleteAll(h);
    printf("delete all: list is empty\n");
    free(h);
    return 0;
}
