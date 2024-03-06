#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <time.h>

#define MAX_LEVEL 20

typedef struct Node {
    int value;
    struct Node** forward;
} Node;

typedef struct SkipList {
    int level;
    Node* header;
} SkipList;

Node* createNode(int value, int level) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->value = value;
    newNode->forward = (Node**)malloc((level + 1) * sizeof(Node*));
    return newNode;
}

SkipList* createSkipList() {
    SkipList* skipList = (SkipList*)malloc(sizeof(SkipList));
    skipList->level = 0;
    skipList->header = createNode(INT_MAX, MAX_LEVEL);
    for (int i = 0; i <= MAX_LEVEL; i++) {
        skipList->header->forward[i] = NULL;
    }
    return skipList;
}

int coinFlip(int layer, int key) {
    if(layer == 0) return 1;
    return (key / (1 << (layer - 1))) & 1;
}
void insert(SkipList* skipList, int value) {
    Node* previous[MAX_LEVEL + 1];
    Node* current = skipList->header;

    for (int i = skipList->level; i >= 0; i--) {
        while (current->forward[i] != NULL && current->forward[i]->value > value) {
            current = current->forward[i];
        }
        previous[i] = current;
    }


    if (coinFlip(skipList->level, value) && coinFlip(skipList->level + 1, value)) {
        previous[skipList->level + 1] = skipList->header;
        skipList->level += 1;
    }

    Node* newNode = createNode(value, skipList->level);

    for (int i = 0; i <= skipList->level; i++) {
        if(!coinFlip(i, value)) continue;
        newNode->forward[i] = previous[i]->forward[i];
        previous[i]->forward[i] = newNode;
    }
}
void del(SkipList* skipList, int value) {
    Node* previous[MAX_LEVEL + 1];
    Node* current = skipList->header;

    for (int i = skipList->level; i >= 0; i--) {
        while (current->forward[i] != NULL && current->forward[i]->value > value) {
            current = current->forward[i];
        }
        previous[i] = current;
    }

    current = current->forward[0];

    if (current != NULL && current->value == value) {
        for (int i = 0; i <= skipList->level; i++) {
            if (previous[i]->forward[i] != current)
                break;
            previous[i]->forward[i] = current->forward[i];
        }

        free(current->forward);
        free(current);
        
        while (skipList->level > 0 && skipList->header->forward[skipList->level] == NULL) {
            skipList->level--;
        }
    }
}
void fastGet(SkipList* skipList, int value) {
    Node* current = skipList->header;
    int flag = 1;
    for (int i = skipList->level; i >= 0; i--) {
        while (current->forward[i] != NULL && current->forward[i]->value > value) {
            printf("%d ", current->forward[i]->value);
            current = current->forward[i];
            flag = 0;
        }
    }
    current = current->forward[0];
    if(current && current->value == value) printf("%d ", value);
    if(flag) printf("-1");
    printf("\n");
}

void slowGet(SkipList* skipList, int value) {
    Node* current = skipList->header;
    if(current == NULL || current->forward[0] == NULL
    || current->forward[0]->value < value){
        printf("-1\n");
        return;
    }
    while (current->forward[0] != NULL && current->forward[0]->value > value) {
        printf("%d ", current->forward[0]->value);
        current = current->forward[0];
    }
    current = current->forward[0];
    if(current && current->value == value) printf("%d ", value);
    printf("\n");
}

void printSkipList(SkipList* skipList) {
    printf("Skip List:\n");
    for (int i = skipList->level; i >= 0; i--) {
        Node* current = skipList->header->forward[i];
        printf("Level %d: ", i);
        while (current != NULL) {
            printf("%d ", current->value);
            current = current->forward[i];
        }
        printf("\n");
    }
    printf("\n");
}

int main() {
    SkipList* skipList = createSkipList();

    int m, t, k;
    scanf("%d", &m);
    for(int i = 0; i < m; i++){
        scanf("%d%d", &t, &k);
        if(t == 1) slowGet(skipList, k);
        else if(t == 2) fastGet(skipList, k);
        else if(t == 3) insert(skipList, k);
        else del(skipList, k);
        #ifdef debug
        printSkipList(skipList);
        #endif
    }
    return 0;

    // Clean up
    free(skipList->header);
    free(skipList);

    return 0;
}
