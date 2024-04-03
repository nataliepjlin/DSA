#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
typedef struct down_t{
    int v;
    struct down_t *next;
}down_t;
typedef struct info_t{
    down_t *down_h, *down_cur;
    int i;//ith v of its direct up
    int cnt;//cnt of its direct downs
    int max_idx, cur_idx;
    long long *presums;
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
void extend_down(info_t info[], int u, int v){
    down_t *dwn = gen_down(v);
    if(info[u].down_h == NULL) info[u].down_h = dwn;
    else info[u].down_cur->next = dwn;
    info[v].i = info[u].cnt;
    info[u].cnt += 1;
    info[u].down_cur = dwn;
}
void pop_down(info_t *info){
    down_t *next = info->down_h->next;
    if(info->cur_idx == info->max_idx) info->max_idx = -1;
    info->cur_idx += 1;
    free(info->down_h);
    info->down_h = next;
}
void pcalloc(info_t *inf){
    inf->presums = calloc(inf->cnt, sizeof(long long));
}
int main(){
    int n, m, q, u, v, LOG = 0, op;
    long long len;
    scanf("%d%d%d", &n, &m, &q);
    while((1 << LOG) <= n) LOG++;
    LOG++;
    up_t up[n][LOG];
    info_t *info = calloc(n,  sizeof(info_t));
    for(int i = 0; i < m; i++){
        scanf("%d%d%lld", &u, &v, &len);
        up[v][0].u = u;
        up[v][0].len = len;
        extend_down(info, u, v);
    }//set direct ancestor and descendants
    up[0][0].u = 0, up[0][0].len = 0;
    for(int i = 0; i < n; i++){
        if(!info[i].cnt){
            info[i].max_idx = -1;
            long long nowsum = 0; int idx = i;
            while(idx != (u = up[idx][0].u)){
                #ifdef debug
                printf("idx = %d, ", idx);
                #endif
                nowsum += up[idx][0].len;
                if(info[u].presums == NULL){
                    #ifdef debug
                    printf("mallocing %d's presum\n", u);
                    #endif
                    pcalloc(&info[u]);
                    info[u].max_idx = -1;
                }
                if(nowsum > info[u].presums[ info[idx].i ])
                    info[u].presums[ info[idx].i ] = nowsum;
                if(info[u].max_idx == -1
                || info[u].presums[ info[u].max_idx ] < nowsum
                || (info[u].presums[ info[u].max_idx ] == nowsum && info[u].max_idx < info[idx].i))
                    info[u].max_idx = info[idx].i;
                #ifdef debug
                printf("%d's %dth presum = %lld, max_idx = %d\n", u, info[idx].i, info[u].presums[ info[idx].i ], info[u].max_idx);
                #endif
                idx = up[idx][0].u;
            }
        }
        for(int j = 1; j < LOG; j++){
            up[i][j].u = up[ up[i][j - 1].u ][j - 1].u;
            up[i][j].len = up[ up[i][j - 1].u ][j - 1].len + up[i][j - 1].len;
        }
    }//binary lifting

    int cur = 0;
    long long ti, pi;
    for(int i = 0; i < q; i++){
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
                int low = 0, high = LOG - 1, idx = ans;
                long long len = 0; 
                while(low <= high){
                    int mid = (low + high) >> 1;
                    if(up[idx][mid].len <= ti){
                        ans = up[idx][mid].u;
                        len = up[idx][mid].len;
                        low = mid + 1;
                    }
                    else high = mid - 1;
                }
                ti -= len;
            }
            printf("%d\n", ans);
        }
        else if(op == 4){
            if(info[cur].max_idx != -1) printf("%lld\n", info[cur].presums[ info[cur].max_idx ]);
            else{
                if(info[cur].down_h == NULL){
                    printf("0\n");
                }
                else{//just being removed but still have down paths
                    info[cur].max_idx = info[cur].cur_idx;
                    for(int i = info[cur].cur_idx + 1; i < info[cur].cnt; i++){
                        if(info[cur].presums[i] >= info[cur].presums[ info[cur].max_idx ])
                            info[cur].max_idx = i;
                    }
                    printf("%lld\n", info[cur].presums[ info[cur].max_idx ]);
                }
            }
        }
        else break;
    }
    free(info);
}