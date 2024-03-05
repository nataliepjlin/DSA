#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct Node {
    struct Node *next;
    int data;
} Node;

void slowGet(Node *node, const int data) {
    Node *cur = node;
    int flag = 1;
    while (cur && cur->data >= data) {
        flag = 0;
        printf("%d ", cur->data);
        cur = cur->next;
    }
    if (flag) printf("-1");
    printf("\n");
}

Node *genNode(int data, Node *next) {
    Node *new = (Node *)malloc(sizeof(Node));
    new->data = data, new->next = next;
    return new;
}

Node *insert(Node *node, const int data) {
    Node *newNode = genNode(data, NULL);
    if (node == NULL || data > node->data) {
        newNode->next = node;
        return newNode;
    }

    Node *prev = NULL;
    Node *cur = node;
    while (cur && data <= cur->data) {
        prev = cur;
        cur = cur->next;
    }

    prev->next = newNode;
    newNode->next = cur;
    return node;
}

Node *deleteNode(Node *node, int data) {
    Node *cur = node;
    Node *prev = NULL;

    while (cur && cur->data > data) {
        prev = cur;
        cur = cur->next;
    }

    if (cur && cur->data == data) {
        if (prev == NULL) {
            node = cur->next;
        } else {
            prev->next = cur->next;
        }
        free(cur);
    }

    return node;
}

int main() {
    int m, t, k;
    scanf("%d", &m);
    Node *node = NULL;

    for (int i = 0; i < m; i++) {
        scanf("%d%d", &t, &k);
        if (t == 1) {
            slowGet(node, k);
        } else if (t == 3) {
            node = insert(node, k);
        } else if (t == 4) {
            node = deleteNode(node, k);
        }
    }

    return 0;
}
