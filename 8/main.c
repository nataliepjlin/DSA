#include <stdio.h>
#include <stdlib.h>
#define N 100001
typedef struct Node{
    int data;
    struct Node *next;
}Node;
Node *genNode(int data){
    Node *node = (Node*)malloc(sizeof(Node));
    node->data = data, node->next = NULL;
    return node;
}
void floydCycle(Node *head){
    Node *fast = head, *slow = head;
    printf("%d", fast->data);
    while(fast && fast->next){
        fast = fast->next->next;
        if(fast) printf(" %d", fast->data);
        slow = slow->next;
        if(fast == slow){
            break;
        }
    }
    printf("\n");
}
int main(){
    int n, data;
    scanf("%d", &n);
    int nextIdx[N];
    Node* nodes[N];
    for(int i = 0; i < n; i++){
        scanf("%d%d", &data, &nextIdx[i]);
        nextIdx[i]--;
        nodes[i] = genNode(data);
        #ifdef debug
        printf("nodes[%d]->data = %d, next[%d] = %d\n", i, nodes[i]->data, i, nextIdx[i]);
        #endif
    }
    for(int i = 0; i < n; i++){
        if(nextIdx[i] != -1){
            nodes[i]->next = nodes[nextIdx[i]];
            #ifdef debug
            printf("nodes[%d]->next = nodes[%d]\n", i, nextIdx[i]);
            #endif
        }
    }
    floydCycle(nodes[0]);
}