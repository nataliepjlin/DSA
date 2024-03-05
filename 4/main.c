#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
typedef struct Node{
    struct Node *next;
    struct Node *below;
    int data;
}Node;
void slowGet(Node *node, const int data){
    int flag = 1;
    while(node && data <= node->data){
        flag = 0;
        printf("%d ", node->data);
        node = node->next;
    }
    if(flag) printf("-1");
    printf("\n");
}
Node* insert(Node *node, const int data){
    Node *new = (Node*)malloc(sizeof(Node)), *prev = NULL;
    new->data = data, new->next = NULL;
    if(node == NULL) return new;

    Node *cur = node;
    while(cur && data < cur->data){
        prev = cur;
        cur = cur->next;
    }
    if(prev == NULL){
        new->next = node;
        return new;
    }
    else{
        prev->next = new;
        new->next = cur;
        return node;
    }
}
Node *delete(Node *node, int data){
    if(node == NULL) return NULL;
    
    Node *cur = node, *prev = NULL;
    while(cur && data < cur->data){
        prev = cur;
        cur = cur->next;
    }
    if(cur && cur->data == data){
        if(prev == NULL)
            return node->next;
        prev->next = cur->next;
        return node;
    }
    return NULL;
}
int main(){
    int m, t, k;
    scanf("%d", &m);
    Node *node = NULL;
    for(int i = 0; i < m; i++){
        scanf("%d%d", &t, &k);
        if(t == 1){
            slowGet(node, k);
        }
        else if(t == 3){
            node = insert(node, k);
        }
        else if(t == 4){
            node = delete(node, k);
        }
    }
}