#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
typedef struct down_t{
    int v;
    long long mx;
    struct down_t *prev, *next;
}down_t;
typedef struct info_t{
    down_t *down_h, *down_cur;
    int i;//ith v of its direct up
    int cnt, collected;//cnt of its direct downs
    long long *presums;
}info_t;
typedef struct up_t{
    int u;
    long long len;//len with u
}up_t;
down_t *gen_down(int v, down_t *prev){
    down_t *d = malloc(sizeof(down_t));
    d->v = v, d->next = NULL, d->prev = prev;
    return d;
}
void extend_down(info_t info[], int u, int v){
    down_t *dwn = gen_down(v, info[u].down_cur);
    if(info[u].down_h == NULL) info[u].down_h = dwn;
    else info[u].down_cur->next = dwn;
    info[v].i = info[u].cnt;
    info[u].cnt += 1;
    info[u].down_cur = dwn;
}
void pop_down(info_t *info){
    down_t *next = info->down_h->next;
    free(info->down_h);
    info->down_h = next;
}
void pmalloc(info_t *inf){
    inf->presums = malloc(inf->cnt * sizeof(long long));
}
long long max(long long a, long long b){
    return (a > b) ? a : b;
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
            long long nowsum = 0; int idx = i; bool allset = true;//all children's info are set
            while(allset){
                allset = false;
                nowsum += up[idx][0].len;
                u = up[idx][0].u;
                if(info[u].presums == NULL) pmalloc(&info[u]);
                info[u].presums[ info[idx].i ] = nowsum;
                info[u].collected += 1;
                if(info[u].collected == info[u].cnt){
                    allset = true;
                    down_t *ptr = info[u].down_cur;
                    ptr->mx = info[u].presums[ info[u].cnt - 1 ];
                    ptr = ptr->prev;
                    for(int j = info[u].cnt - 2; j >= 0; j--){
                        ptr->mx = max(ptr->next->mx, info[u].presums[j]);
                        ptr = ptr->prev;
                    }
                    nowsum = info[u].down_h->mx;
                    idx = up[idx][0].u;
                }
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
            if(info[cur].down_h == NULL) printf("0\n");
            else printf("%lld\n", info[cur].down_h->mx);
        }
        else break;
    }
    free(info);
}