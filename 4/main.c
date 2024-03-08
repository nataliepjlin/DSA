#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#define MAXLAYER 65
typedef struct Node{
    struct Node *next, *below;
    long long key;
}Node;
typedef struct skipList{
    int layer;
    Node** heads;
}skipList;
int coinFlip(int layer, long long key) {
    return (layer == 0 || (key / (1ll << (layer - 1))) & 1);
}
int max(int a, int b){
    return a > b ? a : b;
}
Node *genNode(Node *below, Node *next, long long key){
    Node *node = (Node*)malloc(sizeof(Node));
    node->below = below;
    node->next = next;
    node->key = key;
    return node;
}
void initList(skipList *list){
    list->heads = (Node**)malloc(sizeof(Node*) * MAXLAYER);
    Node *below = NULL;
    for(int i = 0; i < MAXLAYER; i++){
        list->heads[i] = genNode(below, NULL, LONG_MAX);
        below = list->heads[i];
    }
    list->layer = 0;
}
void insert(skipList *list, long long key){
    int newLayer = 0;
    while(coinFlip(newLayer + 1, key))
        newLayer++;
    list->layer = max(list->layer, newLayer);
    #ifdef debug
    printf("list->layer = %d\n", list->layer);
    #endif
    Node **previous = (Node**)malloc(sizeof(Node*) * (list->layer + 1));
    int i = list->layer;
    Node *node = list->heads[list->layer];
    while(node != NULL){//perform fastGet
        while(node->next && node->next->key > key)
            node = node->next;
        previous[i] = node;
        i--;
        node = node->below;
    }
    Node *below = NULL;
    for(i = 0; i <= newLayer; i++){
        Node *newNode = genNode(below, previous[i]->next, key);
        previous[i]->next = newNode;
        below = newNode;
    }
    free(previous);
}
void delete(skipList *list, long long key){
    #ifdef debug
    printf("try delete %lld\n", key);
    #endif
    Node **previous = (Node**)malloc(sizeof(Node*) * (list->layer + 1));
    int i = list->layer;
    Node *node = list->heads[list->layer];
    while(node->below != NULL){
        while(node->next && node->next->key > key)
            node = node->next;
        previous[i] = node;
        i--;
        node = node->below;
    }//i > 0
    while(node->next && node->next->key > key){
        node = node->next;
    }
    /*
    here node->key will > key
    , while node->next will either be NULL or <= key
    */
    previous[0] = node;
    #ifdef debug
    printf("previous[0]->key = %lld\n", previous[0]->key);
    #endif
    node = node->next;
    if(node && node->key == key){
        for(int i = 0; i <= list->layer; i++){
            if(previous[i]->next == NULL 
            || previous[i]->next->key != key)
                break;
            previous[i]->next = (previous[i]->next)->next;
        }
        while(list->layer > 0 && list->heads[list->layer]->next == NULL)
            list->layer -= 1;
    }
    //free(node);
    free(previous);
}
void fastGet(skipList *list, long long key){
    Node *node = list->heads[list->layer];
    bool noprint = true;
    while(node){
        if(node->key != LONG_MAX) printf("%lld ", node->key);
        while(node->next && node->next->key >= key){
            printf("%lld ", node->next->key);
            node = node->next;
            noprint = false;
        }
        node = node->below;
    }
    if(noprint) printf("-1");
    printf("\n");
}
void slowGet(skipList *list, long long key){
    Node *node = list->heads[0];
    bool noprint = true;
    while(node){
        if(node->key != LONG_MAX) printf("%lld ", node->key);
        while(node->next && node->next->key >= key){
            printf("%lld ", node->next->key);
            node = node->next;
            noprint = false;
        }
        node = node->below;
    }
    if(noprint) printf("-1");
    printf("\n");
}
void printList(skipList *list){
    for(int i = list->layer; i >= 0; i--){
        printf("Layer %d:", i);
        Node *cur = list->heads[i]->next;
        while(cur){
            printf(" %lld", cur->key);
            cur = cur->next;
        }
        printf("\n");
    }
    printf("\n");
}
int main(){
    skipList list;
    initList(&list);
    int m, t;
    long long k;
    scanf("%d", &m);
    for(int i = 0; i < m; i++){
        scanf("%d%lld", &t, &k);
        if(t == 3) insert(&list, k);
        else if(t == 2) fastGet(&list, k);
        else if(t == 1) slowGet(&list, k);
        else delete(&list, k);
        #ifdef debug
        printList(&list);
        #endif
    }
    free(list.heads);
}
