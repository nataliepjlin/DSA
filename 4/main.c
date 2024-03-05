#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
typedef struct Node{
    struct Node *next;
    struct Node *below;
    int data;
}Node;
void slowGet(Node *node, const int data){
    Node *cur = node;
    int flag = 1;
    while(cur && cur->data >= data){
        flag = 0;
        printf("%d ", cur->data);
        cur = cur->next;
    }
    if(flag) printf("-1");
    printf("\n");
}
Node *genNode(int data, Node *next){
    Node *new = (Node*)malloc(sizeof(Node));
    new->data = data, new->next = NULL;
    return new;
}
Node* insert(Node *node, const int data){
    Node *prev = NULL, *new = genNode(data, NULL);
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
    }
    return node;
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