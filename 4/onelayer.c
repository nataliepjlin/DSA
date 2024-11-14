#include<stdio.h>
#include<stdlib.h>
typedef struct Node{
    long long data;
    struct Node *next;
}Node;
Node *genNode(Node *next, long long data){
    Node *now = (Node *)malloc(sizeof(Node));
    now->data = data, now->next = next;
    return now;
}
Node *insert(Node *head, long long data){//returning new head
    if(head == NULL || data >= head->data){
        return genNode(head, data);
    }
    head->next = insert(head->next, data);
    return head;
}
Node *del(Node *head, long long data){
    if(head == NULL || head->data < data)
        return head;

    if(head->data == data){
        Node *tmp = head->next;
        free(head);
        return tmp;
    }
    head->next = del(head->next, data);
    return head;
}
void slow(Node *head, long long data){
    if(head == NULL || head->data < data){
        printf("-1\n");
        return;
    }
    Node *cur = head;
    while(cur && cur->data >= data){
        printf("%lld ", cur->data);
        cur = cur->next;
    }
    printf("\n");
}
int main(){
    int m, t;
    long long k;
    scanf("%d", &m);
    Node *head = NULL;
    for(int i = 0; i < m; i++){
        scanf("%d%lld", &t, &k);
        if(t == 1)
            slow(head, k);
        else if(t == 3)
            head = insert(head, k);
        else if(t == 4)
            head = del(head, k);
        else break;
    }
    return 0;
}