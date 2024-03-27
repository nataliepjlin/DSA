#include <stdio.h>
#include <stdlib.h>
#define N 1000001
typedef struct node_t {
	int priority, id;
	int deg;
	struct node_t *child;
	struct node_t *parent;
	struct node_t *sib;
} node_t;
typedef struct heap_t {
	node_t *head;
	int job;
} heap_t;
node_t *node_init(int priority, int id){
	node_t *node = (node_t*)malloc(sizeof (node_t));
	node->priority = priority;
	node->id = id;
	node->deg = 0;
	node->sib = node->child = node->parent = NULL;
	return node;
}
heap_t *heap_init(){
	heap_t *heap = (heap_t*)malloc(sizeof (heap_t));
	heap->head = NULL;
	heap->job = 0;
	return heap;
}
node_t *heap_merge(heap_t *heap1, heap_t *heap2){
	if(heap1->head == NULL) return heap2->head;
	if(heap2->head == NULL) return heap1->head;
	node_t *head;
	node_t *cur1 = heap1->head;
	node_t *cur2 = heap2->head;
	node_t *tail;
	if(heap1->head->deg <= heap2->head->deg){
		head = heap1->head;
		cur1 = cur1->sib;
	}
	else{
		head = heap2->head;
		cur2 = cur2->sib;
	}
	tail = head;
	while(cur1 != NULL && cur2 != NULL){
		if(cur1->deg <= cur2->deg){
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
node_t *heap_union(heap_t *original, heap_t *uni){
	node_t *new_head = heap_merge(original, uni);
	original->head = NULL;
	uni->head = NULL;
	if(new_head == NULL)
		return NULL;
	node_t *prev = NULL;
	node_t *cur = new_head;
	node_t *next = new_head->sib;
	while(next != NULL){
		if(cur->deg != next->deg 
		|| (next->sib != NULL && next->sib->deg == cur->deg)){
			prev = cur;
			cur = next;
		}
		else{
			if(cur->priority > next->priority){
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
void heap_insert(heap_t *heap, int priority, int id){
	node_t *node = node_init(priority, id);
	heap_t *tmp = heap_init();
	tmp->head = node;
	heap->head = heap_union(heap, tmp);
	free(tmp);
}
void heap_remove(heap_t *heap, node_t *head, node_t *prev){
	if(head == heap->head) heap->head = head->sib;
	else prev->sib = head->sib;

	node_t *new_head = NULL;
	node_t *child = head->child;

	while(child != NULL){
		node_t *next = child->sib;
		child->sib = new_head;
		child->parent = NULL;
		new_head = child;
		child = next;
	}

	heap_t *tmp = heap_init();
	tmp->head = new_head;
	heap->head = heap_union(heap, tmp);
	free(tmp);
}


node_t *heap_min(heap_t *heap){
	if(heap->head == NULL) return NULL;

	node_t *min = heap->head;
	node_t *min_prev = NULL;
	node_t *next = min->sib;
	node_t *next_prev = min;

	while(next != NULL){
		if(next->priority > min->priority){
			min = next;
			min_prev = next_prev;
		}
		next_prev = next;
		next = next->sib;
	}
	heap_remove(heap, min, min_prev);
	return min;
}
int main(){
    int n, m, op, id, priority, pid1, pid2;
    scanf("%d%d", &n, &m);
    heap_t *heapArr[N];
    for(int i = 0; i < n; i ++){
        heapArr[i] = heap_init();
    }
    for(int i = 0; i < m; i ++){
        scanf("%d", &op);
        if(op == 1){
            scanf("%d%d%d", &id, &priority, &pid1);
            pid1--;
            heap_insert(heapArr[pid1], priority, id);
			heapArr[pid1]->job += 1;
            printf("%d jobs waiting on printer %d\n", heapArr[pid1]->job, pid1 + 1);
        }
        else if(op == 2){
            scanf("%d", &pid1);
            pid1--;
            if(heapArr[pid1]->head == NULL) printf("no documents in queue\n");
            else{
                node_t *mx = heap_min(heapArr[pid1]);
				heapArr[pid1]->job -= 1;
                printf("%d printed\n", mx->id);
                free(mx);
            }
        }
        else{
            scanf("%d%d", &pid1, &pid2);
            pid1--, pid2--;
            heapArr[pid2]->head = heap_union(heapArr[pid1], heapArr[pid2]);
            heapArr[pid2]->job += heapArr[pid1]->job;
            heapArr[pid1]->job = 0;
            printf("%d jobs waiting on printer %d after moving\n", heapArr[pid2]->job, pid2 + 1);
        }
    }
}