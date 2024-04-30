#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
typedef struct Node{
    int p;//parent
    int rank;
}Node;
Node* make_set(int n){
    Node *nodes = malloc(sizeof(Node) * (n + 1));
    for(int i = 1; i <= n; i++){
        nodes[i].p = i;
        nodes[i].rank = 0;
    }
    return nodes;
}
void Link(Node *nodes, const int x, const int y){
    if(nodes[x].rank > nodes[y].rank)
        nodes[y].p = x;
    else{
        nodes[x].p = y;
        if(nodes[x].rank == nodes[y].rank) nodes[y].rank += 1;
    }
}
int Find_set(Node *nodes, const int x){
    if(x != nodes[x].p){
        nodes[x].p = Find_set(nodes, nodes[x].p);
    }
    return nodes[x].p;
}
void Union(Node *nodes, const int x, const int y){
    int v = Find_set(nodes, x), w = Find_set(nodes, y);
    #ifdef debug
    printf("v = %d, w = %d\n", v, w);
    #endif
    if(v != w) Link(nodes, v, w);
}
void Print_path(Node *nodes, const int x){
    int k = x;
    while(nodes[k].p != k){
        printf("%d ", k);
        k = nodes[k].p;
    }
    printf("%d\n", k);
}
int main(){
    int n, m, x, y; 
    char op, br;
    scanf("%d%d", &n, &m);
    Node *nodes = make_set(n);
    for(int i = 0; i < m; i++){
        scanf("%c%c", &br, &op);
        if(op == 'F'){
            scanf("%d", &x);
            Find_set(nodes, x);
        }
        else if(op == 'U'){
            scanf("%d%d", &x, &y);
            Union(nodes, x, y);
        }
        else{
            scanf("%d", &x);
            Print_path(nodes, x);
        }
    }
    free(nodes);
    return 0;
}