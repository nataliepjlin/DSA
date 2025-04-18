#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#define MAXLAYER 65
typedef struct Node{
    struct Node **forward;
    long long key;
}Node;
typedef struct skipList{
    int level;
    Node* header;
}skipList;
int coinFlip(int layer, long long key) {
    return (layer == 0 || (key / (1ll << (layer - 1))) & 1);
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
void fastGet(skipList *list, long long key){
    Node *node = list->header;
    int noprint = 1;
    for(int i = list->level; i >= 0; i--){
        if(node != list->header) printf("%lld ", node->key);
        while(node->forward[i] && key <= node->forward[i]->key){
            printf("%lld ", node->forward[i]->key);
            node = node->forward[i];
            noprint = 0;
        }
    }
    if(noprint) printf("-1");
    printf("\n");
}
void slowGet(skipList *list, long long key){
    Node *node = list->header;
    int noprint = 1;
    while(node->forward[0] && key <= node->forward[0]->key){
        printf("%lld ", node->forward[0]->key);
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
            printf("%lld ", node->forward[i]->key);
            node = node->forward[i];
        }
        printf("\n");
    }
    printf("-----\n");
}
int max(int a, int b){
    return a > b ? a : b;
}
void insert(skipList *list, long long key){
    Node *node = list->header;
    int newLayer = 0;
    while(coinFlip(newLayer + 1, key))
        newLayer++;
    list->level = max(newLayer, list->level);
    Node **previous = (Node**)malloc(sizeof(Node*) * (list->level + 1));
    for(int i = list->level; i >= 0; i--){
        while(node->forward[i] && node->forward[i]->key > key)
            node = node->forward[i];
        previous[i] = node;
    }
    node = (Node*)malloc(sizeof(Node));
    node->key = key;
    node->forward = (Node**)malloc(sizeof(Node*) * (newLayer + 1));
    for(int i = 0; i <= newLayer; i++){
        node->forward[i] = previous[i]->forward[i];
        previous[i]->forward[i] = node;
    }
    free(previous);
}
void del(skipList *list, long long key){
    Node *node = list->header;
    Node **previous = (Node**)malloc(sizeof(Node*) * (list->level + 1));
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
        while(list->level > 0 && list->header->forward[list->level] == NULL)
            list->level -= 1;
    }
    free(previous);
}
int main(){
    skipList list;
    initList(&list);
    int m, t;
    long long k;
    scanf("%d", &m);
    for(int i = 0; i < m; i++){
        scanf("%d%lld", &t, &k);
        if(t == 1) slowGet(&list, k);
        else if(t == 2) fastGet(&list, k);
        else if(t == 3) insert(&list, k);
        else del(&list, k);
        #ifdef debug
        printList(&list);
        #endif
    }
    free(list.header->forward);
    free(list.header);
}