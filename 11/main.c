#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#define N 1000001
typedef struct down_t{
    int v;
    struct down_t *next;
}down_t;
typedef struct info_t{
    down_t *down_h, *down_cur;
    int pid;//path id
    long long presum;
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
void extend_down(info_t *d, int u, int v, long long *pmax, int *pid, long long len){
    down_t *dwn = gen_down(v);
    int cur_pid = d[u].pid;
    if(d[u].down_h == NULL){
        d[u].down_h = dwn;
    }
    else{
        d[u].down_cur->next = dwn;
        *pid += 1;
        pmax[*pid] = d[u].presum;
        cur_pid = *pid;
    }
    pmax[cur_pid] += len;
    #ifdef debug
    printf("len = %lld, pmax[%d] = %lld\n", len, cur_pid, pmax[cur_pid]);
    #endif
    d[v].presum = pmax[cur_pid];
    d[v].pid = cur_pid;
    d->down_cur = dwn;
}
void pop_down(info_t *d, int u, int pid, long long *pmax){
    pmax[pid] = d[u].presum;
    down_t *next = d[u].down_h->next;
    free(d[u].down_h);
    d[u].down_h = next;
    if(d[u].down_h == NULL) d[u].down_cur = NULL;
}
void print_downs(int u, info_t *info){
    down_t *cur = info[u].down_h;
    while(cur){
        printf("(%d[pid = %d], %d[pid = %d])->", u, info[u].pid, cur->v, info[cur->v].pid);
        cur = cur->next;
    }
    printf("X\n");
}
int main(){
    int n, m, q, u, v, LOG = 0, op, pid = 0;
    long long len;
    scanf("%d%d%d", &n, &m, &q);
    up_t **vec = malloc(sizeof(up_t*) * n);
    info_t *info = malloc(sizeof(info_t) * n);
    long long *pmax = malloc(sizeof(long long) * m);//paths' max
    while((1 << LOG) <= n) LOG++;
    LOG++;
    for(int i = 0; i < n; i++){
        vec[i] = malloc(LOG * sizeof(up_t));
        info[i].down_h = info[i].down_cur = NULL;
    }
    info[0].pid = 0, info[0].presum = 0;
    pmax[0] = 0;
    for(int i = 0; i < m; i++){
        scanf("%d%d%lld", &u, &v, &len);
        vec[v][0].u = u;
        vec[v][0].len = len;
        extend_down(info, u, v, pmax, &pid, len);
        #ifdef debug
        print_downs(u, info);
        #endif
    }//set direct ancestor and descendants
    vec[0][0].u = 0, vec[0][0].len = 0;
    for(int i = 0; i < n; i++){
        for(int j = 1; j < LOG; j++){
            vec[i][j].u = vec[ vec[i][j - 1].u ][j - 1].u;
            vec[i][j].len = vec[ vec[i][j - 1].u ][j - 1].len + vec[i][j - 1].len;
        }
    }//binary lifting

    int cur = 0;
    long long ti;
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
                int prev = cur;
                cur = vec[cur][0].u;
                pop_down(info, cur, info[prev].pid, pmax);
                printf("%d\n", cur);
            }
        }
        else if(op == 3){
            scanf("%lld", &ti);
            int ans = cur;
            while(ans != 0 && ti >= vec[ans][0].len){
                int low = 0, high = LOG - 1, tmpcur = ans;
                #ifdef debug
                printf("tmpcur = %d\n", tmpcur);
                #endif
                long long len = 0; 
                while(low <= high){
                    int mid = (low + high) >> 1;
                    if(vec[tmpcur][mid].len <= ti){
                        ans = vec[tmpcur][mid].u;
                        len = vec[tmpcur][mid].len;
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
        else if(op == 4){
            ti = 0;
            down_t *d = info[cur].down_h;
            while(d != NULL){
                #ifdef debug
                printf("info[%d].pid = %d\n", d->v, info[d->v].pid);
                #endif
                if(pmax[ info[d->v].pid ] > ti) ti = pmax[ info[d->v].pid ];
                d = d->next;
            }
            printf("%lld\n", ti);
        }
        else break;
    }
}