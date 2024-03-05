#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
/* Node */
typedef struct Node {
  int data;
  struct Node *next;
}Node;  
Node *genNode(int data, Node *next)
{
  Node *node =
    (Node *)malloc(sizeof(Node));
  assert(node != NULL);
  node->data = data;
  node->next = next;
  return node;
}
Node *insert(Node *head, int data)
{
  Node *current = genNode(data, NULL);
  if (head == NULL) 
    return(current);

  Node *ptr = head;
  Node *last = NULL;
  while (ptr != NULL && ptr->data > data) {
    last = ptr;
    ptr = ptr->next;
  }
  if (last == NULL) {
    current->next = head;
    return(current);
  } else {
    current->next = last->next;
    last->next = current;
    return(head);
  }
}
Node *delete(Node *head, int data)
{
    Node *cur = head;
    Node *prev = NULL;

    while (cur && cur->data > data) {
        prev = cur;
        cur = cur->next;
    }

    if (cur && cur->data == data) {
        if (prev == NULL) {
            head = cur->next;
        } else {
            prev->next = cur->next;
        }
        free(cur);
    }
    return head;
}
void slow(Node *head, int k){
    if(head == NULL || head->data < k){
        printf("-1\n");
        return;
    }
    while(head && head->data > k){
        printf("%d ", head->data);
        head = head->next;
    }
    if(head && head->data == k)
        printf("%d", head->data);
    printf("\n");
}
int main(void)
{
  
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
        head = delete(head, k);
    else break;
  }
  return 0;
}