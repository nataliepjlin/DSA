#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#define MAXLAYER 20
typedef struct Node{
    struct Node **forward;
    int key;
}Node;
typedef struct skipList{
    int level;
    Node* header;
}skipList;
int coinFlip(int layer, int key) {
    return (layer == 0 || (key / (1 << (layer - 1))) & 1);
}
void initList(skipList *list){
    Node *header = (Node *) malloc(sizeof(Node));
    list->header = header;
    header->key = INT_MAX;
    header->forward = (Node **) malloc(sizeof(Node*) * (MAXLAYER));
    for(int i = 0; i < MAXLAYER; i++)
        header->forward[i] = NULL;
    list->level = 0;
}
void fastGet(skipList *list, int key){
    Node *node = list->header;
    int noprint = 1;
    for(int i = list->level; i >= 0; i--){
        if(node != list->header) printf("%d ", node->key);
        while(node->forward[i] && key <= node->forward[i]->key){
            printf("%d ", node->forward[i]->key);
            node = node->forward[i];
            noprint = 0;
        }
    }
    if(noprint) printf("-1");
    printf("\n");
}
void slowGet(skipList *list, int key){
    Node *node = list->header;
    int noprint = 1;
    while(node->forward[0] && key <= node->forward[0]->key){
        printf("%d ", node->forward[0]->key);
        node = node->forward[0];
        noprint = 0;
    }
    if(noprint) printf("-1");
    printf("\n");
}
void printList(skipList *list){
    for(int i = list->level; i >= 0; i--){
        Node *node = list->header;
        printf("Layer %d: ", i);
        while(node->forward[i]){
            printf("%d ", node->forward[i]->key);
            node = node->forward[i];
        }
        printf("\n");
    }
    printf("-----\n");
}
void insert(skipList *list, int key){
    Node *node = list->header;
    Node *previous[MAXLAYER] = {};
    for(int i = list->level; i >= 0; i--){
        while(node->forward[i] && node->forward[i]->key > key)
            node = node->forward[i];
        previous[i] = node;
    }
    node = (Node*)malloc(sizeof(Node));
    node->key = key;
    node->forward = (Node**)malloc(sizeof(Node*) * (MAXLAYER));
    for(int i = 0; i <= list->level && coinFlip(i, key); i++){
        if(i == list->level && coinFlip(i + 1, key)){
            previous[list->level + 1] = list->header;
            list->level += 1;
        }
        node->forward[i] = previous[i]->forward[i];
        previous[i]->forward[i] = node;
    }
}
void del(skipList *list, int key){
    Node *node = list->header;
    Node *previous[MAXLAYER] = {0};
    for(int i = list->level; i >= 0; i--){
        while(node->forward[i] && node->forward[i]->key > key)
            node = node->forward[i];
        previous[i] = node;
    }
    node = node->forward[0];
    if(node && node->key == key){
        for(int i = 0; i <= list->level; i++){
            if(previous[i]->forward[i] != node)
                break;
            previous[i]->forward[i] = node->forward[i];
        }
        free(node);
        if(list->level > 0 && list->header->forward[list->level] == NULL)
            list->level -= 1;
    }
}
int main(){
    skipList list;
    initList(&list);
    int m, t, k;
    scanf("%d", &m);
    for(int i = 0; i < m; i++){
        scanf("%d%d", &t, &k);
        if(t == 1) slowGet(&list, k);
        else if(t == 2) fastGet(&list, k);
        else if(t == 3) insert(&list, k);
        else del(&list, k);
        #ifdef debug
        printList(&list);
        #endif
    }
}