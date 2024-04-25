#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define B 100001
enum COLOR{WHITE, GREY, BLACK};
typedef struct Vertex{
    int color;
    int d, f;
    int parent;
    int *idx;
    int edge_size;
}Vertex;
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
void DFS_visit(Vertex *vertex, int *time, const int u, int *finish_order, int *fi, const bool need_f){
    *time += 1;
    vertex[u].d = *time;
    vertex[u].color = GREY;
    for(int i = 0; i < vertex[u].edge_size; i++){
        if(vertex[ vertex[u].idx[i] ].color == WHITE){
            vertex[ vertex[u].idx[i] ].parent = u;
            DFS_visit(vertex, time, vertex[u].idx[i], finish_order, fi, need_f);
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
    if(need_f){
        finish_order[*fi] = u;
        *fi += 1;
    }
}
void DFS(Vertex *vertex, const int b, int *finish_order){
    int time = 0, fi = 0;
    for(int i = 0; i < b; i++){
        if(vertex[i].color == WHITE) DFS_visit(vertex, &time, i, finish_order, &fi, true);
        #ifdef debug
        printf("%dth vertex time = (%d, %d)\n", i, vertex[i].d, vertex[i].f);
        #endif
    }
}
void getParent(Vertex *trans_v, int cur){
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
    int time = 0, cnt = 0;
    for(int i = 0; i < b; i++){
        int idx = finish_order[b - 1 - i];
        #ifdef debug
        printf("finish_order[%d] = %d\n", b - 1 - i, idx);
        #endif
        if(trans_v[idx].color == WHITE){
            #ifdef debug
            printf("trans_v[%d].color == WHITE\n", idx);
            #endif
            trans_v[idx].parent = -1;
            DFS_visit(trans_v, &time, idx, NULL, NULL, false);
            cnt++;
        }
    }
    printf("%d\n", cnt);
    return 0;
}