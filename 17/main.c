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
void DFS_visit(Vertex *vertex, int *time, const int u){
    *time += 1;
    vertex[u].d = *time;
    vertex[u].color = -1;
    ListNode *node = vertex[u].edgeHead;
    while(node != NULL){
        if(vertex[node->v].color == 0){
            vertex[node->v].parent = u;
            node->pathType = 0;
            DFS_visit(vertex, time, node->v);
        }
        else{
            if(vertex[node->v].color == -1) node->pathType = 1;
            else if(vertex[u].d < vertex[node->v].d) node->pathType = 2;
            else node->pathType = 3;
        }
        node = node->next;
    }
    vertex[u].color = 1;
    *time += 1;
    vertex[u].f = *time;
}
void DFS(Vertex *vertex, const int v){
    int time = 0;
    char dict[] = "TBFC";
    for(int i = 0; i < v; i++){
        if(vertex[i].color == 0) DFS_visit(vertex, &time, i);
        
        printf("%d ", i + 1);
        ListNode *cur = vertex[i].edgeHead;
        while(cur != NULL){
            printf("%c", dict[cur->pathType]);
            cur = cur->next;
        }
        printf("\n");
    }
}
int main(){
    int v, d, n;
    scanf("%d", &v);
    Vertex vertex[v]; 
    for(int i = 0; i < v; i++){
        vertex[i].color = 0, vertex[i].d = 0, vertex[i].edgeHead = vertex[i].edgeTail = NULL;
        vertex[i].f = -1, vertex[i].parent = -1;
        scanf("%d", &d);
        for(int j = 0; j < d; j++){
            scanf("%d", &n);
            insertEdge(&vertex[i], n - 1);
        }
    }
    DFS(vertex, v);
    return 0;
}