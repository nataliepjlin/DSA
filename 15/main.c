#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
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
    int idx;
    struct Time *next;
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
Time* genTime(const int idx, Time *next){
    Time *t = malloc(sizeof(Time));
    t->idx = idx, t->next = next;
    return t;
}
void printTime(Time *order){
    Time *cur = order;
    while(cur != NULL){
        printf("%d->", cur->idx);
        cur = cur->next;
    }
    printf("X\n");
}
void DFS_visit(Vertex *vertex, int *time, const int u, bool needTime,Time **order){
    *time += 1;
    vertex[u].d = *time;
    vertex[u].color = GREY;
    ListNode *node = vertex[u].edgeHead;
    while(node != NULL){
        if(vertex[node->v].color == WHITE){
            vertex[node->v].parent = u;
            DFS_visit(vertex, time, node->v, needTime, order);
        }
        node = node->next;
    }
    vertex[u].color = BLACK;
    *time += 1;
    vertex[u].f = *time;
    if(needTime){
        *order = genTime(u, *order);
        #ifdef debug
        printf("add %d to order list\n", u);
        printTime(*order);
        #endif
    }
}
Time* DFS(Vertex *vertex, const int b){
    int time = 0;
    Time *order = NULL;
    for(int i = 0; i < b; i++){
        if(vertex[i].color == WHITE) DFS_visit(vertex, &time, i, true, &order);
        #ifdef debug
        printf("%dth vertex time = (%d, %d)\n", i, vertex[i].d, vertex[i].f);
        #endif
    }
    return order;
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
    Vertex vertex[b], trans_v[b]; 
    for(int i = 0; i < b; i++){
        vertex[i].color = WHITE;
        vertex[i].parent = -1;
        vertex[i].edgeHead = vertex[i].edgeTail = NULL;
        trans_v[i].color = WHITE;
        trans_v[i].parent = -1;
        trans_v[i].edgeHead = trans_v[i].edgeTail = NULL;
    }
    for(int i = 0; i < a; i++){
        scanf("%d%d", &u, &v);
        u--, v--;
        insertEdge(&vertex[u], v);
        insertEdge(&trans_v[v], u);
    }
    Time *order = DFS(vertex, b);
    int time = 0;
    for(int i = 0; i < b; i++){
        int idx = order->idx;
        if(trans_v[idx].color == WHITE) DFS_visit(trans_v, &time, idx, false, NULL);
        Time *next = order->next;
        free(order);
        order = next;
    }
    int cnt = 0;
    for(int i = 0; i < b; i++){
        condense(trans_v, i);
        cnt += (trans_v[i].parent == -1);
    }
    printf("%d\n", cnt);
    return 0;
}