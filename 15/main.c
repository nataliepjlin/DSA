#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#define B 100001
enum COLOR{WHITE, GREY, BLACK};
typedef struct Vertex{
    int color;
    int d, f;
    int parent;
    int *idx;
    int edge_size;
}Vertex;
typedef struct RootNodes{
    int *roots;
    int size;
}RootNodes;
void rnodes_push_rnode(RootNodes *rnodes, const int idx){
    rnodes->size += 1;
    rnodes->roots = realloc(rnodes->roots, rnodes->size * (sizeof(int)));
    rnodes->roots[rnodes->size - 1] = idx;
}
void dfs2(Vertex *trans_v, const int u, const int root){//add every node's root parent
    trans_v[u].color = GREY;
    for(int i = 0; i < trans_v[u].edge_size; i++){
        if(trans_v[ trans_v[u].idx[i] ].color == WHITE){
            trans_v[ trans_v[u].idx[i] ].parent = root;
            dfs2(trans_v, trans_v[u].idx[i], root);
        }
    }
    trans_v[u].color = BLACK;
}
void insertEdge(Vertex *u, const int v){
    u->edge_size += 1;
    u->idx = realloc(u->idx, sizeof(int) * (u->edge_size));
    u->idx[ u->edge_size - 1  ] = v;
}
void printEdge(Vertex *vertex, const int b){
    for(int i = 0; i < b; i++){
        printf("%d:", i);
        for(int j = 0; j < vertex[i].edge_size; j++){
            printf(" %d", vertex[i].idx[j]);
        }
        printf("\n");
    }
}
void DFS_visit(Vertex *vertex, int *time, const int u, int *finish_order, int *fi){
    *time += 1;
    vertex[u].d = *time;
    vertex[u].color = GREY;
    for(int i = 0; i < vertex[u].edge_size; i++){
        if(vertex[ vertex[u].idx[i] ].color == WHITE){
            vertex[ vertex[u].idx[i] ].parent = u;
            DFS_visit(vertex, time, vertex[u].idx[i], finish_order, fi);
        }
    }
    vertex[u].color = BLACK;
    *time += 1;
    vertex[u].f = *time;
    if(vertex[u].edge_size != 0){
        free(vertex[u].idx);
        vertex[u].idx = NULL;
        vertex[u].edge_size = 0;
    }
    finish_order[*fi] = u;
    *fi += 1;
}
void DFS(Vertex *vertex, const int b, int *finish_order){
    int time = 0, fi = 0;
    for(int i = 0; i < b; i++){
        if(vertex[i].color == WHITE) DFS_visit(vertex, &time, i, finish_order, &fi);
        #ifdef debug
        printf("%dth vertex time = (%d, %d)\n", i, vertex[i].d, vertex[i].f);
        #endif
    }
}
void destroy_rnodes(RootNodes *rnodes){
    if(rnodes->size == 0) return;
    free(rnodes->roots);
}
int cmp(const void *a, const void *b){
    return *(int*)a - *(int*)b;
}
int main(){
    int b, a, mode, u, v;//b: number of vertices, a: number of edges
    scanf("%d%d%d", &b, &a, &mode);
    Vertex vertex[B] = {0}, trans_v[B] = {0};
    int finish_order[b];
    for(int i = 0; i < a; i++){
        scanf("%d%d", &u, &v);
        u--, v--;
        insertEdge(&vertex[u], v);
        insertEdge(&trans_v[v], u);
    }
    #ifdef debug
    printEdge(vertex, b);
    printEdge(trans_v, b);
    #endif
    DFS(vertex, b, finish_order);
    int time = 0;
    RootNodes rnodes; rnodes.size = 0, rnodes.roots = NULL;
    for(int i = 0; i < b; i++){//2nd dfs
        int idx = finish_order[b - 1 - i];
        if(trans_v[idx].color == WHITE){
            rnodes_push_rnode(&rnodes, idx);
            trans_v[idx].parent = rnodes.size - 1;//group index
            dfs2(trans_v, idx, rnodes.size - 1);
        }
    }
    printf("%d\n", rnodes.size);
    if(mode == 1) return 0; 
    for(int i = 0; i < b; i++){
        #ifdef debug
        printf("trans_v[%d], parent = %d, edge = %d\n", i, trans_v[i].parent, trans_v[i].edge_size);
        #endif
        for(int j = 0; j < trans_v[i].edge_size
        && rnodes.roots[ trans_v[i].parent ] != -1; j++){//rnodes.roots[ trans_v[i].parent ] != -1: check whether ith node's parent has been explored or not
            #ifdef debug
            printf("trans_v[%d].parent = %d\n", trans_v[i].idx[j], trans_v[ trans_v[i].idx[j] ].parent);
            #endif
            if(trans_v[ trans_v[i].idx[j] ].parent + 1 == trans_v[i].parent){//trans_v[i].idx[j]: ith node's neighbor; +1: check if they are adjacent SCCs
                rnodes.roots[ trans_v[i].parent ] = -1;
            }
        }
        if(trans_v[i].idx != NULL){
            free(trans_v[i].idx);
            trans_v[i].idx = NULL;
            trans_v[i].edge_size = 0;
        }
    }
    int valid = 1;
    for(int i = rnodes.size - 1; i > 0 && valid; i--){
        valid = (rnodes.roots[i] == -1);
    }
    printf("%d\n", valid);
    destroy_rnodes(&rnodes);
    return 0;
}
