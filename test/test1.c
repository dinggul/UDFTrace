#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct listNode_{
    int val;
    struct listNode_ *next;
} listNode;

typedef struct listHead_ {
    listNode *list;
} listHead;

listHead* newList() {
    listHead *res = (listHead *)malloc(sizeof(listHead));
    res->list = NULL;

    return res;
}

int listLen(listHead *l) {
    int res = 0;
    listNode* node = l->list;

    while (node != NULL) {
        res++;
        node = node->next;
    }

    return res;
}

void addItem(listHead *l, int v) {
    listNode *newOne = (listNode *)malloc(sizeof(listNode));
    newOne->val = v;

    if (l->list == NULL) {
        l->list = newOne;
        return;
    }

    listNode *node = l->list;
    while (node->next != NULL) node = node->next;

    node->next = newOne;
}

int getItem(listHead *l, int v) {
    int len = listLen(l);
    if (len <= v) return -1;

    listNode *node = l->list;
    while (v) {
        node = node->next;
        v--;
    }

    return node->val;
}

int fibo(listHead *f, int n) {
    int res = getItem(f, n);
    if (res != -1) return res;

    res = fibo(f, n-1) + fibo(f, n-2);
    assert(listLen(f) == n);
    addItem(f, res);
    return res;
}

int main(int argc, char *argv[]) {
    listHead *fiboS = newList();
    addItem(fiboS, 0);
    addItem(fiboS, 1);
    addItem(fiboS, 1);
    addItem(fiboS, 2); // fib[0] = 0, fib[1] = 1, fib[2] = 1, fib[3] = 2;

    int arg;
    if (argc < 2) arg = 44;
    else arg = atoi(argv[1]);

    printf("Fibo[%d]: ", arg);
    printf("%d\n", fibo(fiboS, arg));
    return 0;
}
