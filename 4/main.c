#include<stdio.h>
#include<stdlib.h>
typedef struct Node{
    int data;
    struct Node *next;
}Node;
Node *insert(Node *head, int data){//returning new head
    Node *now = (Node *)malloc(sizeof(Node));
    now->data = data, now->next = NULL;
    if(head == NULL || data >= head->data){
        now->next = head;
        return now;
    }
    head->next = insert(head->next, data);
    return head;
}
Node *del(Node *head, int data){
    if(head == NULL)
        return NULL;

    Node *tmp;
    if(head->data == data){
        tmp = head->next;
        free(head);
        return tmp;
    }
    else if(head->data > data)
        head->next = del(head->next, data);
    return head;
}
void slow(Node *head, int data){
    if(head == NULL || head->data < data){
        printf("-1\n");
        return;
    }
    Node *cur = head;
    while(cur && cur->data >= data){
        printf("%d ", cur->data);
        cur = cur->next;
    }
    printf("\n");
}
int main(){
    int m, t, k;
    scanf("%d", &m);
    Node *head = NULL;
    for(int i = 0; i < m; i++){
        scanf("%d%d", &t, &k);
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