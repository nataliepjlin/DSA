#include <stdio.h>
#include <stdlib.h>
typedef struct Node{
    int data;
    struct Node *next;
}Node;
Node *genNode(int data){
    Node *node = (Node*)malloc(sizeof(Node));
    node->data = data, node->next = NULL;
}
void floydCycle(Node *head){
    Node *fast = head, *slow = head;
    while(fast && fast->next){
        printf("%d ", fast->data);
        fast = fast->next->next;
        slow = slow->next;
        if(fast == slow){
            printf("%d", fast->data);
            break;
        }
    }
    printf("\n");
}
int main(){
    int n, data;
    scanf("%d", &n);
    int nextIdx[n];
    Node* nodes[n];
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