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
typedef struct RootNode{
    int id, visit;
    int *children, child_cnt;
    int *colors, color_cnt;
}RootNode;
typedef struct RootNodes{
    RootNode *roots;
    int size;
}RootNodes;
RootNodes *gen_rnodes(){
    RootNodes *rnodes = malloc(sizeof(RootNodes));
    rnodes->roots = NULL, rnodes->size = 0;
    return rnodes;
}
void rnodes_push_rnode(RootNodes *rnodes){
    rnodes->size += 1;
    rnodes->roots = realloc(rnodes->roots, rnodes->size * (sizeof(RootNode)));
    rnodes->roots[rnodes->size - 1].id = rnodes->size - 1;
    rnodes->roots[rnodes->size - 1].child_cnt = rnodes->roots[rnodes->size - 1].color_cnt = 0;
    rnodes->roots[rnodes->size - 1].children = rnodes->roots[rnodes->size - 1].colors = NULL;
}
void rnode_push_child(RootNode **rnode, const int idx){
    (*rnode)->child_cnt += 1;
    (*rnode)->children = realloc((*rnode)->children, (*rnode)->child_cnt * (sizeof(int)));
    (*rnode)->children[ (*rnode)->child_cnt - 1 ] = idx;
}
void rnode_push_color(RootNode *rnode, const int c){
    rnode->color_cnt += 1;
    rnode->colors = realloc(rnode->colors, rnode->color_cnt * (sizeof(int)));
    rnode->colors[rnode->color_cnt - 1] = c;
}
void dfs2(Vertex *trans_v, const int u, RootNode *rnode){//add every node's root parent
    trans_v[u].color = GREY;
    for(int i = 0; i < trans_v[u].edge_size; i++){
        if(trans_v[ trans_v[u].idx[i] ].color == WHITE){
            trans_v[ trans_v[u].idx[i] ].parent = rnode->id;
            dfs2(trans_v, trans_v[u].idx[i], rnode);
        }
    }
    rnode_push_child(&(rnode), u);
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
    for(int i = 0; i < rnodes->size; i++){
        if(rnodes->roots[i].colors) free(rnodes->roots[i].colors);
    }
    free(rnodes->roots);
}
int cmp(const void *a, const void *b){
    return *(int*)a - *(int*)b;
}
void printColor(RootNode *rnode){
    printf("%d:", rnode->id);
    for(int i = 0; i < rnode->color_cnt; i++){
        printf(" %d", rnode->colors[i]);
    }
    printf("\n");
}
void condense(RootNode *rnode){
    int i = 0, j = 0;
    while(j < rnode->color_cnt){
        if(rnode->colors[i] == rnode->colors[j]) j++;
        else{
            i++;
            rnode->colors[i] = rnode->colors[j];
            j++;
        }
    }
    rnode->color_cnt = i + 1;
}
void dfs_topo(RootNode *roots, const int u, int *finish_order, int *fi){
    roots[u].visit = GREY;
    for(int i = roots[u].color_cnt - 1; i >= 0; i--){
        if(roots[ roots[u].colors[i] ].visit == WHITE){
            dfs_topo(roots, roots[u].colors[i], finish_order, fi);
        }
    }
    roots[u].visit = BLACK;
    finish_order[*fi] = u;
    *fi += 1;
}
int topo(RootNodes *rnodes){
    int finish_order[rnodes->size], fi = 0;
    dfs_topo(rnodes->roots, rnodes->size - 1, finish_order, &fi);
    if(fi != rnodes->size) return 0;
    for(int i = rnodes->size - 1; i >= 0; i--){
        if(finish_order[i] != i) return 0;
    }
    return 1;
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
    DFS(vertex, b, finish_order);
    int time = 0;
    RootNodes rnodes; rnodes.size = 0, rnodes.roots = NULL;
    for(int i = 0; i < b; i++){//2nd dfs
        int idx = finish_order[b - 1 - i];
        if(trans_v[idx].color == WHITE){
            rnodes_push_rnode(&rnodes);
            trans_v[idx].parent = rnodes.size - 1;
            rnodes.roots[rnodes.size - 1].visit = WHITE;
            dfs2(trans_v, idx, &(rnodes.roots[rnodes.size - 1]));
            #ifdef debug
            printf("SCC #%d:", rnodes.size - 1);
            for(int j = 0; j < rnodes.roots[rnodes.size - 1].child_cnt; j++){
                printf(" %d", rnodes.roots[rnodes.size - 1].children[j]);
            }
            printf("\n");
            #endif
        }
    }
    printf("%d\n", rnodes.size);
    if(mode == 1) return 0; 
    for(int i = 0; i < rnodes.size; i++){
        for(int j = 0; j < rnodes.roots[i].child_cnt; j++){
            const int cid = rnodes.roots[i].children[j];
            for(int k = 0; k < trans_v[cid].edge_size; k++){
                if(trans_v[ trans_v[cid].idx[k] ].parent != i){
                    rnode_push_color(&(rnodes.roots[i]), trans_v[ trans_v[cid].idx[k] ].parent);
                }
            }
            if(trans_v[cid].idx != NULL){//clean trans_v's edges
                free(trans_v[cid].idx);
                trans_v[cid].idx = NULL;
                trans_v[cid].edge_size = 0;
            }
        }
        if(rnodes.roots[i].children != NULL){//clean roots's children
            free(rnodes.roots[i].children);
            rnodes.roots[i].children = NULL;
            rnodes.roots[i].child_cnt = 0;
        }
        if(rnodes.roots[i].color_cnt != 0){
            qsort(rnodes.roots[i].colors, rnodes.roots[i].color_cnt, sizeof(int), cmp);
            condense(&rnodes.roots[i]);
        }
        #ifdef debug
        printColor(&rnodes.roots[i]);
        #endif
    }
    printf("%d\n", topo(&rnodes));
    destroy_rnodes(&rnodes);
    return 0;
}