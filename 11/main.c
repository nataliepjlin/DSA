#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
typedef struct down_t{
    int v; long long mx;
    struct down_t *prev, *next;//prev for op4
}down_t;
typedef struct treasure_seq treasure_seq;
typedef struct treasure_t{
    long long val;//val if reach 0
    int negpos;
    struct treasure_t *next, *prev;//prev for op2
    treasure_seq *seq;
}treasure_t;
struct treasure_seq{
    int top_id, btm_id;//dungeon id where top & btm are located
    treasure_t *top, *btm;
};
typedef struct info_t{
    down_t *down_h, *down_cur;
    int i;//ith v of its direct up
    int cnt, collected, depth;//cnt of its direct downs
    long long *presums;
    treasure_seq *seq;//treasure sequence
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
    if(next != NULL) next->prev = NULL;
    free(info->down_h);
    info->down_h = next;
}
void pmalloc(info_t *inf){
    inf->presums = malloc(inf->cnt * sizeof(long long));
}
long long max(long long a, long long b){
    return (a > b) ? a : b;
}
int op3(long long *ti, int cur, const int LOG, up_t up[][LOG], info_t *info){
    int ans = cur;
    while(ans != 0 && *ti >= up[ans][0].len){
        int low = 0, high = LOG - 1, idx = ans;
        long long len = 0; 
        while(low <= high){
            int mid = (low + high) >> 1;
            if(up[idx][mid].len <= *ti){
                ans = up[idx][mid].u;
                len = up[idx][mid].len;
                low = mid + 1;
            }
            else high = mid - 1;
        }
        *ti -= len;
    }
    return ans;
}
treasure_t *gen_t(long long val, int negpos, treasure_seq *seq, treasure_t *prev){
    treasure_t *t = malloc(sizeof(treasure_t));
    t->next = NULL, t->seq = seq, t->prev = prev;
    t->val = val, t->negpos = negpos;
    return t;
}
treasure_seq *gen_seq(long long val, int negpos, int depth, int cur){
    treasure_seq *seq = malloc(sizeof(treasure_seq));
    seq->top_id = seq->btm_id = cur;
    treasure_t *t = gen_t(val, negpos, seq, NULL);
    seq->top = t, seq->btm = t;
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
    up[0][0].u = 0, up[0][0].len = 0, info[0].depth = -1;
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
        info[i].depth = info[ up[i][0].u ].depth + 1;
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
                if(info[cur].seq != NULL){
                    if(info[cur].seq->top_id == cur){
                        free(info[cur].seq->top);
                        free(info[cur].seq);
                    }
                    else{
                        treasure_t *prev = info[cur].seq->btm->prev;
                        free(info[cur].seq->btm);
                        info[cur].seq->btm = prev;
                        info[cur].seq->btm->next = NULL;
                        info[ up[cur][0].u ].seq = info[cur].seq;
                    }
                    info[cur].seq = NULL;
                }
                cur = up[cur][0].u;
                pop_down(&info[cur]);
                printf("%d\n", cur);
            }
        }
        else if(op == 3){
            scanf("%lld", &ti);
            int ans = op3(&ti, cur, LOG, up, info);
            printf("%d\n", ans);
        }
        else if(op == 4){
            if(info[cur].down_h == NULL) printf("0\n");
            else printf("%lld\n", info[cur].down_h->mx);
        }
        else if(op == 5){
            scanf("%lld", &pi);
            int furthest = op3(&pi, cur, LOG, up, info);
            int negpos = up[ furthest ][0].u - (furthest == 0);
            if(info[cur].seq == NULL){
                u = up[cur][0].u;
                if(info[u].seq != NULL){
                    info[cur].seq = info[u].seq;
                    treasure_t *t = gen_t(pi, negpos, info[cur].seq, info[cur].seq->btm);
                    info[cur].seq->btm->next = t, info[cur].seq->btm = t;
                    info[cur].seq->btm_id = cur;
                    info[u].seq = NULL;
                }
                else{
                    info[cur].seq = gen_seq(pi, negpos, info[cur].depth, cur);
                }
            }//no merging or pushing seq up occurs here
            else{
                treasure_t *t = gen_t(pi, negpos, info[cur].seq, info[cur].seq->btm);
                info[cur].seq->btm->next = t, info[cur].seq->btm = t;
                info[cur].seq->btm_id = cur;
                u = up[ info[cur].seq->top_id ][0].u;//lift top_id by 1 step
                if(info[ up[u][0].u ].seq != NULL){
                    info[ up[u][0].u ].seq->btm->next = info[cur].seq->top;
                    info[cur].seq->top = info[ up[u][0].u ].seq->top;
                    info[cur].seq->top_id = info[ up[u][0].u ].seq->top_id;
                    free(info[ up[u][0].u ].seq);
                }//u(top's new position) == 0 won't be qualified for this part
                else{
                    if(u == 0){
                        if(info[cur].seq->top->negpos != -1) printf("value lost at %d\n", info[cur].seq->top->negpos);
                        else printf("value remaining is %lld\n", info[cur].seq->top->val);
                        treasure_t *next = info[cur].seq->top->next;
                        free(info[cur].seq->top);
                        info[cur].seq->top = next, info[cur].seq->top->prev = NULL;
                    }
                    else info[cur].seq->top_id = u;
                }
            }
        }
        else break;
    }
    free(info);
}