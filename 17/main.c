#include <stdio.h>
#include <stdlib.h>
#define V 1001
typedef struct ListNode{
    int v, pathType;
    struct ListNode *next;
}ListNode;
typedef struct Vertex{
    int color;//-1: grey, 0: white, 1: black
    int d, f;
    int parent;
    ListNode *edgeHead, *edgeTail;
}Vertex;
ListNode *genNode(const int v, ListNode *next){
    ListNode *node = malloc(sizeof(ListNode));
    node->v = v, node->pathType = -1, node->next = next;
    return node;
}
void insertEdge(Vertex *u, int v){
    ListNode *node = genNode(v, NULL);
    if(u->edgeHead == NULL) u->edgeHead = node;
    else u->edgeTail->next = node;
    u->edgeTail = node;
}
void printEdge(Vertex *u){
    ListNode *cur = u->edgeHead;
    while(cur != NULL){
        printf("%d->", cur->v + 1);
        cur = cur->next;
    }
    printf("X\n");
}
void DFS_visit(Vertex *vertex, int *time, const int u){
    *time += 1;
    vertex[u].d = *time;
    vertex[u].color = -1;
    ListNode *node = vertex[u].edgeHead;
    while(node != NULL){
        if(vertex[node->v].color == 0){
            vertex[node->v].parent = u;
            node->pathType = 0;
            #ifdef debug
            printf("set edge (%d, %d) to type %d\n", u + 1, node->v + 1, node->pathType);
            #endif
            DFS_visit(vertex, time, node->v);
        }
        else{
            if(vertex[node->v].color == -1) node->pathType = 1;
            else if(vertex[u].d < vertex[node->v].d) node->pathType = 2;
            else if(vertex[u].d > vertex[node->v].d) node->pathType = 3;
            #ifdef debug
            printf("set edge (%d, %d) to type %d\n", u + 1, node->v + 1, node->pathType);
            #endif
        }
        node = node->next;
    }
    vertex[u].color = 1;
    *time += 1;
    vertex[u].f = *time;
}
void DFS(Vertex *vertex, const int v){
    int time = 0;
    for(int i = 0; i < v; i++){
        if(vertex[i].color == 0) DFS_visit(vertex, &time, i);
    }
}
int main(){
    int v, d, n;
    Vertex vertex[V] = {0}; 
    scanf("%d", &v);
    for(int i = 0; i < v; i++){
        scanf("%d", &d);
        for(int j = 0; j < d; j++){
            scanf("%d", &n);
            insertEdge(&vertex[i], n - 1);
        }
        #ifdef debug
        printEdge(&vertex[i]);
        #endif
    }
    DFS(vertex, v);
    char dict[] = "TBFC";
    for(int i = 0; i < v; i++){
        printf("%d ", i + 1);
        ListNode *cur = vertex[i].edgeHead;
        while(cur != NULL){
            printf("%c", dict[cur->pathType]);
            cur = cur->next;
        }
        printf("\n");
    }
}