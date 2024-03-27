#include <stdio.h>
#include <stdlib.h>
#define N 1000001
enum OP{ADD = 1, PRINT, MOVE};
typedef struct node_t{
    int id, deg;
    int priorty;//value in most ref
    struct node_t *child, *parent, *sib;
}node_t;
typedef struct heap_t{
    node_t *head;
    int numOfJob;
}heap_t;
node_t *genNode(int id, int priorty){
    node_t *node = malloc(sizeof(node_t));
    node->id = id, node->priorty = priorty;
    node->deg = 0;
    node->child = node->parent = node->sib = NULL;
    return node;
}
heap_t *genHeap(){
    heap_t *heap = (heap_t*)malloc(sizeof(heap_t));
    heap->head = NULL;
    heap->numOfJob = 0;
    return heap;
}
node_t *heapMerge(heap_t *h1, heap_t *h2){
    if(h1->head == NULL) return h2->head;
    if(h2->head == NULL) return h1->head;
    node_t *head, *cur1 = h1->head, *cur2 = h2->head, *tail;
    if(h1->head->deg >= h2->head->deg){
        head = h1->head;
        cur1 = cur1->sib;
    }
    else{
        head = h2->head;
        cur2 = cur2->sib;
    }
    tail = head;
    while(cur1 && cur2){
        if(cur1->deg >= cur2->deg){
            tail->sib = cur1;
            cur1 = cur1->sib;
        }
        else{
            tail->sib = cur2;
            cur2 = cur2->sib;
        }
        tail = tail->sib;
    }
    tail->sib = (cur1 != NULL) ? cur1 : cur2;
    return head;
}
node_t *heapUnion(heap_t *origin, heap_t *uni){
    node_t *new_head = heapMerge(origin, uni);
    origin->head = NULL, uni->head = NULL;
    if(new_head == NULL) return NULL;
    node_t *prev = NULL, *cur = new_head, *next = new_head->sib;
    while(next != NULL){
        if(cur->deg != next->deg || (next->sib && next->sib->deg == cur->deg)){
            prev = cur;
            cur = next;
        }
        else{
            if(cur->priorty > next->priorty){
                cur->sib = next->sib;
                next->parent = cur;
                next->sib = cur->child;
                cur->child = next;
                cur->deg += 1;
            }
            else{
                if(prev == NULL) new_head = next;
                else prev->sib = next;
                cur->parent = next;
                cur->sib = next->child;
                next->child = cur;
                next->deg += 1;
                cur = next;
            }
        }
        next = cur->sib;
    }
    return new_head;
}
void heapInsert(heap_t *h, int id, int priorty){
    node_t *node = genNode(id, priorty);
    heap_t *tmp = genHeap();
    tmp->head = node;
    h->head = heapUnion(h, tmp);
    h->numOfJob += 1;
    free(tmp);
}
void heapRemove(heap_t *h, node_t *mx, node_t *prev){
    if(mx == h->head) h->head = mx->sib;
    else prev->sib = mx->sib;
    node_t *newHead = NULL, *child = mx->child;
    while(child){//reversing
        node_t *next = child->sib;
        child->sib = newHead;//newHead stores previous sibling
        child->parent = NULL;
        newHead = child;
        child = next;
    }
    heap_t *tmp = genHeap();
    tmp->head = newHead;
    h->head = heapUnion(h, tmp);
    h->numOfJob -= 1;
    free(tmp);
}
node_t *heapMax(heap_t *h){
    node_t *max = h->head;
    node_t *max_prev = NULL, *next = max->sib, *next_prev = max;
    while(next){
        if(next->priorty > max->priorty){
            max = next;
            max_prev = next_prev;
        }
        next_prev = next;
        next = next->sib;
    }
    heapRemove(h, max, max_prev);
    return max;
}
int main(){
    int n, m, op, id, priority, pid1, pid2;
    scanf("%d%d", &n, &m);
    heap_t *heapArr[N];
    for(int i = 0; i < n; i++){
        heapArr[i] = genHeap();
    }
    for(int i = 0; i < m; i++){
        scanf("%d", &op);
        if(op == ADD){
            scanf("%d%d%d", &id, &priority, &pid1);
            pid1--;
            heapInsert(heapArr[pid1], id, priority);
            printf("%d jobs waiting on printer %d\n", heapArr[pid1]->numOfJob, pid1 + 1);
        }
        else if(op == PRINT){
            scanf("%d", &pid1);
            pid1--;
            if(heapArr[pid1]->head == NULL) printf("no documents in queue\n");
            else{
                node_t *mx = heapMax(heapArr[pid1]);
                printf("%d printed\n", mx->id);
                // free(mx);
            }
        }
        else{
            scanf("%d%d", &pid1, &pid2);
            pid1--, pid2--;
            heapArr[pid2]->head = heapUnion(heapArr[pid1], heapArr[pid2]);
            heapArr[pid2]->numOfJob += heapArr[pid1]->numOfJob;
            heapArr[pid1]->numOfJob = 0;
            printf("%d jobs waiting on printer %d after moving\n", heapArr[pid2]->numOfJob, pid2 + 1);
        }
    }
}