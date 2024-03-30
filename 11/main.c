#include <stdio.h>
#include <stdlib.h>
#define N 1000001
typedef struct down_t{
    int v;
    struct down_t *next;
}down_t;
typedef struct big_down{
    down_t *down_h, *down_cur;
}big_down;
typedef struct info_t{
    int u;
    long long len;//len with u
}info_t;
down_t *gen_down(int v){
    down_t *d = malloc(sizeof(down_t));
    d->v = v, d->next = NULL;
    return d;
}
void extend_down(big_down *d, int v){
    down_t *dwn = gen_down(v);
    if(d->down_h == NULL) d->down_h = dwn;
    else d->down_cur->next = dwn;
    d->down_cur = dwn;
}
void pop_down(big_down *d){
    d->down_h = d->down_h->next;
    if(d->down_h == NULL) d->down_cur = NULL;
}
void print_downs(int u, big_down *bd){
    down_t *cur = bd->down_h;
    while(cur){
        printf("(%d, %d)->", u, cur->v);
        cur = cur->next;
    }
    printf("X\n");
}
int main(){
    int n, m, q, u, v, LOG = 0, op;
    long long len;
    scanf("%d%d%d", &n, &m, &q);
    info_t **vec = malloc(sizeof(info_t*) * n);
    big_down downs[n];
    while((1 << LOG) <= n) LOG++;
    LOG++;
    for(int i = 0; i < n; i++){
        vec[i] = malloc(LOG * sizeof(info_t));
        downs[i].down_h = downs[i].down_cur = NULL;
    }
    for(int i = 0; i < m; i++){
        scanf("%d%d%lld", &u, &v, &len);
        vec[v][0].u = u;//vth dun's 0th(direct) ancestor
        vec[v][0].len = len;
        extend_down(&downs[u], v);
        #ifdef debug
        print_downs(u, &downs[u]);
        #endif
    }//set direct ancestor and descendants
    vec[0][0].u = 0, vec[0][0].len = 0;
    for(int i = 0; i < n; i++){
        #ifdef debug
        printf("%dth dun 1st ancestor = %d, len = %lld\n", i, vec[i][0].u, vec[i][0].len);
        #endif
        for(int j = 1; j < LOG; j++){
            vec[i][j].u = vec[ vec[i][j - 1].u ][j - 1].u;
            vec[i][j].len = vec[ vec[i][j - 1].u ][j - 1].len + vec[i][j - 1].len;
            #ifdef debug
            printf("%dth dun 2^%dth ancestor = %d, len = %lld\n", i, j, vec[i][j].u, vec[i][j].len);
            #endif
        }
        #ifdef debug
        printf("\n");
        #endif
    }//binary lifting

    int cur = 0;
    long long ti;
    for(int i = 0; i < q; i++){
        scanf("%d", &op);
        if(op == 1){
            if(downs[cur].down_h == NULL) printf("-1\n");
            else{
                cur = downs[cur].down_h->v;
                printf("%d\n", cur);
            }
        }
        else if(op == 2){
            if(cur == 0) printf("-1\n");
            else{
                cur = vec[cur][0].u;
                pop_down(&downs[cur]);
                printf("%d\n", cur);
            }
        }
        else if(op == 3){
            scanf("%lld", &ti);
            int low = 0, high = LOG - 1, ans = cur;
            while(low <= high){
                int mid = (low + high) >> 1;
                if(vec[cur][mid].len <= ti){
                    ans = vec[cur][mid].u;
                    low = mid + 1;
                }
                else high = mid - 1;
            }
            printf("%d\n", ans);
        }
        else break;
    }
}