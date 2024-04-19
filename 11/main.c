#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
typedef struct down_t{
    int v; long long mx;
    struct down_t *prev, *next;//prev is for op4
}down_t;
typedef struct treasure_t{
    long long val;//val if reach 0
    int negpos;
    struct treasure_t *next, *prev;//prev for op2
}treasure_t;
typedef struct treasure_queue{
    treasure_t *top, *btm;
    int size;
}treasure_queue;
typedef struct info_t{
    down_t *down_h, *down_cur;
    int u; long long len;
    int i;//ith v of its direct up
    int cnt, collected;//cnt of its direct downs
    long long *presums;
    bool hasU;
    int tcnt;
}info_t;
down_t *gen_down(const int v, down_t *prev){
    down_t *d = malloc(sizeof(down_t));
    d->v = v, d->next = NULL, d->prev = prev, d->mx = -1;
    return d;
}
void extend_down(info_t *info, const int u, const int v){
    down_t *dwn = gen_down(v, info[u].down_cur);
    if(info[u].down_h == NULL) info[u].down_h = dwn;
    else info[u].down_cur->next = dwn;
    info[v].i = info[u].cnt;
    info[u].cnt += 1;
    info[u].down_cur = dwn;
}
void remove_down(info_t *info, const int id){
    down_t *next = info[id].down_h->next;
    free(info[id].down_h);
    if(next != NULL) next->prev = NULL;
    else info[id].down_cur = NULL;
    info[id].down_h = next;
}
void destroy_down(info_t *info, const int id){
    down_t *d = info[id].down_h;
    while(d != NULL){
        down_t *next = d->next;
        free(d);
        d = next;
    }
    info[id].down_h = NULL;
    info[id].down_cur = NULL;
}
void destroy_dq(treasure_queue *q){
    treasure_t *t = q->top;
    while(t != NULL){
        treasure_t *next = t->next;
        free(t);
        t = next;
    }
    q->top = NULL, q->btm = NULL;
    free(q);
    q = NULL;
}
void destroy_info(info_t *info, const int n){
    for(int i = 0; i < n; i++){
        destroy_down(info, i);
    }
    free(info);
}
long long max(long long a, long long b){
    return (a > b) ? a : b;
}
void set_furthest_descendant(info_t *info, const int cur){
    long long nowsum = 0; 
    int idx = cur; 
    bool ready = true;//all children's info are set
    while(ready && info[idx].hasU){
        #ifdef debug
        printf("idx = %d\n", idx);
        #endif
        ready = false;
        nowsum += info[idx].len;
        int u = info[idx].u;
        if(info[u].presums == NULL) info[u].presums = malloc(info[u].cnt * sizeof(long long));
        info[u].presums[ info[idx].i ] = nowsum;
        #ifdef debug
        printf("info[%d].presums[%d] = %lld\n", u, info[idx].i, info[u].presums[ info[idx].i ]);
        #endif
        info[u].collected += 1;
        if(info[u].collected == info[u].cnt){
            ready = true;
            #ifdef debug
            printf("info[%d]'s child info collecting job complete\n", u);
            #endif
            down_t *ptr = info[u].down_cur;
            ptr->mx = info[u].presums[ info[u].cnt - 1 ];
            ptr = ptr->prev;
            for(int j = info[u].cnt - 2; j >= 0; j--){
                ptr->mx = max(ptr->next->mx, info[u].presums[j]);
                ptr = ptr->prev;
            }
            free(info[u].presums);
            info[u].presums = NULL;
            nowsum = info[u].down_h->mx;
            idx = info[idx].u;
        }
    }
}
typedef struct dun_t{
    long long presum;
    int id;
}dun_t;
typedef struct vec_t{
    dun_t *duns;
    int size;
}vec_t;
vec_t *gen_vec(int n){
    vec_t *vec = malloc(sizeof(vec_t));
    vec->duns = malloc(sizeof(dun_t) * n);
    vec->duns[0].id = 0, vec->duns[0].presum = 0;
    vec->size = 1;
    return vec;
}
void destroy_vec(vec_t *vec){
    free(vec->duns);
    free(vec);
}
void push_back(vec_t *vec, const int id, const long long len){
    vec->size += 1;
    vec->duns[ vec->size - 1 ].id = id;
    vec->duns[ vec->size - 1 ].presum = vec->duns[ vec->size - 2 ].presum + len;
}
void pop_back(vec_t *vec){
    vec->size -= 1;
}
void print_vec(vec_t *vec){
    printf("\nprinting vec: ");
    for(int i = 0; i < vec->size; i++){
        printf("(%d, %lld)->", vec->duns[i].id, vec->duns[i].presum);
    }
    printf("X\n");
}
int op3(vec_t *vec, const long long val){
    int low = 0, high = vec->size - 1, cur = vec->size - 1;
    int ans = high;
    while(low <= high){
        int mid = (low + high) >> 1;
        long long dist = vec->duns[cur].presum - vec->duns[mid].presum;
        if(dist <= val){
            ans = mid;
            high = mid - 1;
        }
        else low = mid + 1;
    }
    return ans;
}
treasure_t *gen_t(long long val, int negpos, treasure_t *prev){
    treasure_t *t = malloc(sizeof(treasure_t));
    t->next = NULL, t->prev = prev;
    t->val = val, t->negpos = negpos;
    return t;
}
treasure_queue *gen_dq(long long val, int negpos){
    treasure_queue *q = malloc(sizeof(treasure_queue));
    treasure_t *t = gen_t(val, negpos, NULL);
    q->top = t, q->btm = t;
    q->size = 1;
    return q;
}
void print_dq(treasure_queue *dq){
    printf("dq->size = %d\n", dq->size);
    treasure_t *t = dq->top;
    while(t->next != NULL){
        printf("(%lld, %d)--", t->val, t->negpos);
        t = t->next;
    }
    assert(t == dq->btm);
    printf("(%lld, %d)\n", t->val, t->negpos);
}
int main(){
    int n, m, q;
    scanf("%d%d%d", &n, &m, &q);
    info_t *info = calloc(n, sizeof(info_t));
    int u, v; long long len;
    for(int i = 0; i < m; i++){
        scanf("%d%d%lld", &u, &v, &len);
        info[v].u = u, info[v].len = len;
        info[v].hasU = true;
        extend_down(info, u, v);
        #ifdef debug
        printf("info[%d]'s %dth down = %d\n", u, info[u].cnt - 1, info[u].down_cur->v);
        printf("info[%d].u = %d, len = %lld\n", v, info[v].u, info[v].len);
        #endif
    }
    info[0].u = -1;
    for(int i = n - 1; i >= 0; i--){
        if(info[i].hasU && !info[i].cnt){
            set_furthest_descendant(info, i);
        }
    }
    
    vec_t *vec = gen_vec(n);
    int cur = 0, op; long long val;
    treasure_queue *dq = NULL;
    for(int i = 0; i < q; i++){
        #ifdef debug
        print_vec(vec);
        #endif
        scanf("%d", &op);
        if(op == 1){
            if(info[cur].down_h == NULL) printf("-1\n");
            else{
                cur = info[cur].down_h->v;
                push_back(vec, cur, info[cur].len);
                printf("%d\n", cur);
            }
        }
        else if(op == 2){
            if(cur == 0) printf("-1\n");
            else{
                if(info[cur].tcnt > 0){
                    if(dq->size == 1) destroy_dq(dq);
                    else{
                        treasure_t *prev = dq->btm->prev;
                        free(dq->btm);
                        dq->btm = prev;
                        dq->btm->next = NULL;
                        dq->size -= 1;
                        info[ info[cur].u ].tcnt += (info[cur].tcnt - 1);
                    }
                }
                destroy_down(info, cur);
                cur = info[cur].u;
                remove_down(info, cur);
                pop_back(vec);
                printf("%d\n", cur);
            }
        }
        else if(op == 3){
            scanf("%lld", &val);
            int ans = op3(vec, val);
            printf("%d\n", vec->duns[ans].id);
        }
        else if(op == 4){
            if(info[cur].down_h == NULL) printf("0\n");
            else printf("%lld\n", info[cur].down_h->mx);
        }
        else if(op == 5){
            scanf("%lld", &val);
            if(cur == 0){
                printf("value remaining is %lld\n", val);
                continue;
            }
            int ret = op3(vec, val);
            int negpos = info[ vec->duns[ret].id ].u;
            if(negpos == -1) val -= vec->duns[ vec->size - 1 ].presum;
            info[cur].tcnt += 1;
            if(dq == NULL) dq = gen_dq(val, negpos);
            else{
                treasure_t *t = gen_t(val, negpos, dq->btm);
                dq->btm->next = t, dq->btm = t;
                dq->size += 1;
                if(dq->size == vec->size){
                    if(dq->top->negpos != -1) printf("value lost at %d\n", dq->top->negpos);
                    else printf("value remaining is %lld\n", dq->top->val);
                    treasure_t *next = dq->top->next;
                    free(dq->top);
                    dq->top = next, dq->top->prev = NULL;
                    dq->size -= 1;
                }
            }
        }
        else{
            scanf("%d%lld", &v, &len);
            info[v].u = cur, info[v].len = len;
            extend_down(info, cur, v);
            info[cur].down_cur->mx = len + ((info[v].down_h == NULL) ? 0ll : info[v].down_h->mx);
            down_t *d = info[cur].down_cur->prev;
            while(d != NULL && d->mx < info[cur].down_cur->mx){
                d->mx = info[cur].down_cur->mx;
                d = d->prev;
            }
        }
        #ifdef debug
        if(dq != NULL) print_dq(dq);
        #endif
    }
    destroy_vec(vec);
    destroy_info(info, n);
    if(dq != NULL) destroy_dq(dq);
    return 0;
}