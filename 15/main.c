#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define B 100001
enum COLOR{WHITE, GREY, BLACK};
typedef struct Vertex{
    int color;
    int parent;
    int *idx;
    int edge_size;
}Vertex;
typedef struct RootNode{
    int self_idx;
    int *children, child_cnt;
    int *colors, color_cnt;
}RootNode;
typedef struct RootNodes{
    RootNode *roots;
    int size;
}RootNodes;
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
void DFS_visit(Vertex *vertex, const int u, int *finish_order, int *fi){
   vertex[u].color = GREY;
    for(int i = 0; i < vertex[u].edge_size; i++){
        if(vertex[ vertex[u].idx[i] ].color == WHITE){
            vertex[ vertex[u].idx[i] ].parent = u;
            DFS_visit(vertex, vertex[u].idx[i], finish_order, fi);
        }
    }
    vertex[u].color = BLACK;
    finish_order[*fi] = u;
    *fi += 1;
}
RootNodes *genRootNodes(){
    RootNodes *root_nodes = malloc(sizeof(RootNodes));
    root_nodes->roots = NULL, root_nodes->size = 0;
    return root_nodes;
}
void rnodes_push_back(RootNodes *rnodes, const int idx){
    rnodes->size += 1;
    rnodes->roots = realloc(rnodes->roots, rnodes->size * (sizeof(RootNode)));
    RootNode rnode = rnodes->roots[ rnodes->size - 1 ];
    rnode.child_cnt = 0, rnode.color_cnt = 0;
    rnode.children = NULL, rnode.colors = NULL;
    rnode.self_idx = idx;
}
void rnode_push_child(RootNode *rnode, const int idx){
    rnode->child_cnt += 1;
    rnode->children = realloc(rnode->children, rnode->child_cnt * (sizeof(int)));
    rnode->children[ rnode->child_cnt - 1 ] = idx;
}
void dfs2(Vertex *vertex, const int u, RootNode *rnode){
    vertex[u].color = GREY;
    for(int i = 0; i < vertex[u].edge_size; i++){
        if(vertex[ vertex[u].idx[i] ].color == WHITE){
            rnode_push_child(rnode, vertex[u].idx[i]);
            vertex[ vertex[u].idx[i] ].parent = u;
            dfs2(vertex, vertex[u].idx[i], rnode);
        }
    }
    vertex[u].color = BLACK;
}
void DFS(Vertex *vertex, const int b, int *finish_order){
    int fi = 0;
    for(int i = 0; i < b; i++){
        if(vertex[i].color == WHITE) DFS_visit(vertex, i, finish_order, &fi);
    }
}
int main(){
    int b, a, mode, u, v;
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
    DFS(vertex, b, finish_order);//1st dfs
    
    RootNodes *root_nodes = genRootNodes();
    for(int i = 0; i < b; i++){//2nd dfs
        int idx = finish_order[b - 1 - i];
        #ifdef debug
        printf("finish_order[%d] = %d\n", b - 1 - i, idx);
        #endif
        if(trans_v[idx].color == WHITE){
            rnodes_push_back(root_nodes, idx);
            dfs2(trans_v, idx, &(root_nodes->roots[ root_nodes->size - 1 ]));
            trans_v[idx].parent = -1;
            #ifdef debug
            printf("trans_v[%d].color == WHITE, child:", idx);
            RootNode rnode = root_nodes->roots[ root_nodes->size - 1 ];
            for(int j = 0; j < rnode.child_cnt; j++){
                printf(" %d", rnode.children[j]);
            }
            printf("\n");
            #endif
        }
    }
    printf("%d\n", root_nodes->size);
    return 0;
}