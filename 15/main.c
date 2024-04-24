#include <stdio.h>
#include <stdlib.h>
#define B 100001
enum COLOR{WHITE, GREY, BLACK};
typedef struct ListNode{
    int v;
    struct ListNode *next;
}ListNode;
typedef struct Vertex{
    int color;
    int d, f;
    int parent;
    ListNode *edgeHead, *edgeTail;
}Vertex;
typedef struct Time{
    int idx, *f;
}Time;
ListNode *genNode(const int v){
    ListNode *node = malloc(sizeof(ListNode));
    node->v = v, node->next = NULL;
    return node;
}
void insertEdge(Vertex *u, const int v){
    ListNode *node = genNode(v);
    if(u->edgeHead == NULL) u->edgeHead = node;
    else u->edgeTail->next = node;
    u->edgeTail = node;
}
void DFS_visit(Vertex *vertex, int *time, const int u){
    *time += 1;
    vertex[u].d = *time;
    vertex[u].color = GREY;
    ListNode *node = vertex[u].edgeHead;
    while(node != NULL){
        if(vertex[node->v].color == WHITE){
            vertex[node->v].parent = u;
            DFS_visit(vertex, time, node->v);
        }
        node = node->next;
    }
    vertex[u].color = BLACK;
    *time += 1;
    vertex[u].f = *time;
}
void DFS(Vertex *vertex, const int b){
    int time = 0;
    for(int i = 0; i < b; i++){
        if(vertex[i].color == WHITE) DFS_visit(vertex, &time, i);
        #ifdef debug
        printf("%dth vertex time = (%d, %d)\n", i, vertex[i].d, vertex[i].f);
        #endif
    }
}
int cmp(const void *a, const void *b){
    Time *pa = (Time*)a, *pb = (Time*)b;
    return *(pb->f) - *(pa->f);
}
void printEdge(Vertex *vertex, const int b){
    for(int i = 0; i < b; i++){
        ListNode *cur = vertex[i].edgeHead;
        printf("%dth vertex, parent = %d, edges: ", i, vertex[i].parent);
        while(cur != NULL){
            printf("%d->", cur->v);
            cur = cur->next;
        }
        printf("X\n");
    }
}
void printTime(Time *finishOrder, const int b){
    for(int i = 0; i < b; i++)
        printf("(%d, %d)%c", finishOrder[i].idx, *(finishOrder[i].f), " \n"[i == b - 1]);
}
void condense(Vertex *trans_v, int cur){
    int root = cur;
    for(; trans_v[root].parent >= 0; root = trans_v[root].parent);
    while(cur != root){
        int parent = trans_v[cur].parent;
        trans_v[cur].parent = root;
        cur = parent;
    }
}
int main(){
    int b, a, mode, u, v;
    scanf("%d%d%d", &b, &a, &mode);
    if(mode == 2) return 0;
    Vertex vertex[b], trans_v[b]; 
    Time finishOrder[b];//sorted by finish time from large to small 
    for(int i = 0; i < b; i++){
        vertex[i].color = WHITE;
        vertex[i].parent = -1;
        vertex[i].edgeHead = vertex[i].edgeTail = NULL;
        trans_v[i].color = WHITE;
        trans_v[i].parent = -1;
        trans_v[i].edgeHead = trans_v[i].edgeTail = NULL;
        finishOrder[i].idx = i, finishOrder[i].f = &(vertex[i].f);
    }
    for(int i = 0; i < a; i++){
        scanf("%d%d", &u, &v);
        u--, v--;
        insertEdge(&vertex[u], v);
        insertEdge(&trans_v[v], u);
    }
    DFS(vertex, b);
    #ifdef debug
    printEdge(vertex, b);
    printTime(finishOrder, b);
    #endif
    qsort(finishOrder, b, sizeof(Time), cmp);
    #ifdef debug
    printTime(finishOrder, b);
    #endif
    int time = 0;
    for(int i = 0; i < b; i++){
        int idx = finishOrder[i].idx;
        if(trans_v[idx].color == WHITE) DFS_visit(trans_v, &time, idx);
    }
    #ifdef debug
    printEdge(trans_v, b);
    #endif
    int cnt = 0;
    for(int i = 0; i < b; i++){
        condense(trans_v, i);
        cnt += (trans_v[i].parent == -1);
    }
    printf("%d\n", cnt);
    return 0;
}