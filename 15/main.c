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
    int parent, id;
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
void DFS_visit(Vertex *vertex, int *time, const int u, Time **order){
    *time += 1;
    vertex[u].d = *time;
    vertex[u].color = GREY;
    ListNode *node = vertex[u].edgeHead;
    while(node != NULL){
        if(vertex[node->v].color == WHITE){
            vertex[node->v].parent = u;
            DFS_visit(vertex, time, node->v, order);
        }
        node = node->next;
    }
    vertex[u].color = BLACK;
    *time += 1;
    vertex[u].f = *time;
    *order = genTime(u, *order);
}
Time* DFS(Vertex *vertex, const int b){
    int time = 0;
    Time *order = NULL;
    for(int i = 0; i < b; i++){
        if(vertex[i].color == WHITE) DFS_visit(vertex, &time, i, &order);
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
bool noDup(Vertex *u, const int v){
    ListNode *cur = u->edgeHead;
    while(cur != NULL){
        if(cur->v == v) return false;
    }
    return true;
}
void topo_DFS(Vertex *root_nodes, int *cnt, const int u){
    root_nodes[u].color = GREY;
    ListNode *node = root_nodes[u].edgeHead;
    int prev = -1;
    while(node != NULL){
        if(node->v != prev && root_nodes[node->v].color == WHITE){
            root_nodes[node->v].parent = u;
            topo_DFS(root_nodes, cnt, node->v);
        }
        prev = node->v;
        node = node->next;
    }
    root_nodes[u].color = WHITE;
    *cnt += 1;
}
int topo(Vertex *root_nodes, const int cnt){
    for(int i = 0; i < cnt; i++){
        if(root_nodes[i].color == WHITE){
            int cur = 0;
            topo_DFS(root_nodes, &cur, i);
            #ifdef debug
            printf("cur = %d\n", cur);
            #endif
            if(cur == cnt) return 1;
        }
    }
    return 0;
}
int main(){
    int b, a, mode, u, v;
    scanf("%d%d%d", &b, &a, &mode);
    Vertex vertex[b], trans_v[b], root_nodes[b];
    for(int i = 0; i < b; i++){
        vertex[i].color = WHITE;
        vertex[i].parent = -1;
        vertex[i].edgeHead = vertex[i].edgeTail = NULL;
        trans_v[i].color = WHITE;
        trans_v[i].parent = -1;
        trans_v[i].edgeHead = trans_v[i].edgeTail = NULL;
        root_nodes[i].color = WHITE;
        root_nodes[i].parent = -1;
        root_nodes[i].edgeHead = root_nodes[i].edgeTail = NULL;
    }
    for(int i = 0; i < a; i++){
        scanf("%d%d", &u, &v);
        u--, v--;
        insertEdge(&vertex[u], v);
        insertEdge(&trans_v[v], u);
    }
    Time *order = DFS(vertex, b);//1st dfs
    int time = 0, cnt = 0;
    for(int i = 0; i < b; i++){//2nd dfs
        int idx = order->idx;
        if(trans_v[idx].color == WHITE){
            Time *dfs2 = NULL;
            DFS_visit(trans_v, &time, idx, &dfs2);
            Time *next = dfs2->next;
            free(dfs2), dfs2 = next;
            #ifdef debug
            printf("%dth color = %d->", cnt, idx);
            #endif
            while(dfs2 != NULL){
                trans_v[ dfs2->idx ].parent = idx;
                #ifdef debug
                printf("%d->", dfs2->idx);
                #endif
                next = dfs2->next;
                free(dfs2), dfs2 = next;
            }
            #ifdef debug
            printf("X\n");
            #endif
            trans_v[idx].color = cnt;
            cnt++;
        }
        Time *next = order->next;
        free(order);
        order = next;
    }
    printf("%d\n", cnt);
    if(mode == 2){
        int roots[cnt], rcnt = 0;
        for(int i = 0; i < b; i++){//collapsing
            int myParent = (trans_v[i].parent == -1) ? i : trans_v[i].parent;
            ListNode *pointAtMe = trans_v[i].edgeHead;
            while(pointAtMe != NULL){
                int pParent = (trans_v[pointAtMe->v].parent == -1) ? pointAtMe->v : trans_v[pointAtMe->v].parent;
                if(pParent != myParent){
                    insertEdge(&root_nodes[ trans_v[myParent].color ], trans_v[pParent].color);
                    #ifdef debug
                    printf("insert root edge for (%d, %d)\n", myParent, pParent);
                    #endif
                }
                pointAtMe = pointAtMe->next;
            }
        }
        printf("%d\n", topo(root_nodes, cnt));
    }
    return 0;
}