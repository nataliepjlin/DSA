#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
typedef struct down_t{
    int v;
    struct down_t *next;
}down_t;
typedef struct info_t{
    down_t *down_h, *down_cur;
    bool has;
    long long treasure;
}info_t;
typedef struct up_t{
    int u;
    long long len;//len with u
}up_t;
down_t *gen_down(int v){
    down_t *d = malloc(sizeof(down_t));
    d->v = v, d->next = NULL;
    return d;
}
void extend_down(info_t *d, int v){
    down_t *dwn = gen_down(v);
    if(d->down_h == NULL) d->down_h = dwn;
    else d->down_cur->next = dwn;
    d->down_cur = dwn;
}
void pop_down(info_t *d){
    down_t *next = d->down_h->next;
    free(d->down_h);
    d->down_h = next;
    if(d->down_h == NULL) d->down_cur = NULL;
}
void print_downs(int u, info_t *bd){
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
    while((1 << LOG) <= n) LOG++;
    LOG++;
    up_t up[n][LOG];
    info_t info[n];
    memset(info, 0, sizeof(info));
    for(int i = 0; i < m; i++){
        scanf("%d%d%lld", &u, &v, &len);
        up[v][0].u = u;
        up[v][0].len = len;
        extend_down(&info[u], v);
    }//set direct ancestor and descendants
    up[0][0].u = 0, up[0][0].len = 0;
    for(int i = 0; i < n; i++){
        for(int j = 1; j < LOG; j++){
            up[i][j].u = up[ up[i][j - 1].u ][j - 1].u;
            up[i][j].len = up[ up[i][j - 1].u ][j - 1].len + up[i][j - 1].len;
        }
    }//binary lifting

    int cur = 0;
    long long ti, pi;
    for(int i = 0; i < q; i++){
        #ifdef debug
        printf("\ncur = %d\n", cur);
        #endif
        scanf("%d", &op);
        if(op == 1){
            if(info[cur].down_h == NULL) printf("-1\n");
            else{
                cur = info[cur].down_h->v;
                printf("%d\n", cur);
            }
        }
        else if(op == 2){
            if(cur == 0) printf("-1\n");
            else{
                cur = up[cur][0].u;
                pop_down(&info[cur]);
                printf("%d\n", cur);
            }
        }
        else if(op == 3){
            scanf("%lld", &ti);
            int ans = cur;
            while(ans != 0 && ti >= up[ans][0].len){
                int low = 0, high = LOG - 1, tmpcur = ans;
                #ifdef debug
                printf("tmpcur = %d\n", tmpcur);
                #endif
                long long len = 0; 
                while(low <= high){
                    int mid = (low + high) >> 1;
                    if(up[tmpcur][mid].len <= ti){
                        ans = up[tmpcur][mid].u;
                        len = up[tmpcur][mid].len;
                        low = mid + 1;
                    }
                    else high = mid - 1;
                }
                #ifdef debug
                printf("len = %lld, ans = %d\n", len, ans);
                #endif
                ti -= len;
            }
            printf("%d\n", ans);
        }
        else if(op == 5){
            scanf("%lld", &pi);
            int idx = cur, neg = 0;
            while(idx != 0){
                if(info[idx].has == false){
                    info[idx].treasure = pi;
                    info[idx].has = true;
                    break;
                }
                long long old = info[idx].treasure;
                info[idx].treasure = pi;
                if(old < up[idx][0].len){
                    if(!neg) neg = up[idx][0].u;
                    pi = -1;
                }
                else pi = old - up[idx][0].len;
                idx = up[idx][0].u;
            }
            if(idx == 0){
                if(pi >= 0) printf("value remaining is %lld\n", pi);
                else printf("value lost at %d\n", neg);
            }
        }
        else break;
    }
}