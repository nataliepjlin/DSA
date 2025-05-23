#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
typedef struct qnode_t{
    int i;//ith down
    long long val;
    struct qnode_t *prev, *next;
}qnode_t;
typedef struct deque_t{
    qnode_t *front, *end;
}deque_t;
qnode_t *gen_qnode(const int i, const long long val, qnode_t *prev, qnode_t *next){
    qnode_t *node = malloc(sizeof(qnode_t));
    node->i = i, node->val = val, node->next = next, node->prev = prev;
    return node;
}
deque_t *gen_dq(){
    deque_t *dq = malloc(sizeof(deque_t));
    dq->front = dq->end = NULL;
    return dq;
}
void push_end(const int i, const long long val, deque_t *dq){
    while(dq->end != NULL && dq->end->val <= val){
        qnode_t *prev = dq->end->prev;
        free(dq->end);
        dq->end = prev;
    }
    qnode_t *new = gen_qnode(i, val, dq->end, NULL);
    if(dq->end == NULL) dq->front = new;
    else dq->end->next = new;
    dq->end = new;
}
void pop_front(deque_t *dq){
    qnode_t *next = dq->front->next;
    free(dq->front);
    dq->front = next;
    if(dq->front == NULL) dq->end = NULL;
}
void push_front(const int i, const long long val, deque_t *dq){
    qnode_t *new = gen_qnode(i, val, NULL, dq->front);
    if(dq->front == NULL){
        dq->front = dq->end = new;
        return;
    }
    dq->front->prev = new;
    dq->front = new;
}
void print_dq(deque_t *dq){
    qnode_t *node = dq->front;
    printf("\nprinting dq: ");
    while(node != NULL){
        printf("%d %lld, ", node->i, node->val);
        node = node->next;
    }
    printf("\n");
}
void destroy_dq(deque_t *dq){
    qnode_t *node = dq->front;
    while(node != NULL){
        qnode_t *next = node->next;
        free(node);
        node = next;
    }
    dq->front = dq->end = NULL;
    free(dq);
}
typedef struct treasure_t{
    long long val;//val if reach 0
    int negpos;
    struct treasure_t *next, *prev;//prev for op2
}treasure_t;
typedef struct treasure_seq{
    int top_id, btm_id;//dungeon id where top & btm are located
    treasure_t *top, *btm;
}treasure_seq;
typedef struct down_t{
    int v;
    struct down_t *prev, *next;//prev is for op4
}down_t;
typedef struct info_t{
    down_t *down_h, *down_cur;
    int u; long long len;
    int i;//ith v of its direct up
    int cnt, collected;//cnt of its direct downs
    long long *presums;
    deque_t *mx;
    treasure_seq *seq;//treasure sequence
    bool hasU;
}info_t;
down_t *gen_down(const int v, down_t *prev){
    down_t *d = malloc(sizeof(down_t));
    d->v = v, d->next = NULL, d->prev = prev;
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
void destroy_seq(treasure_seq *seq){
    treasure_t *t = seq->top;
    while(t != NULL){
        treasure_t *next = t->next;
        free(t);
        t = next;
    }
    seq->top = NULL, seq->btm = NULL;
    free(seq);
}
void destroy_info(info_t *info, const int n){
    for(int i = 0; i < n; i++){
        destroy_down(info, i);
        if(info[i].seq != NULL){
            destroy_seq(info[i].seq);
            info[i].seq = NULL;
        }
        if(info[i].mx != NULL){
            destroy_dq(info[i].mx);
            info[i].mx = NULL;
        }
    }
    free(info);
}
long long max(long long a, long long b){
    return (a > b) ? a : b;
}
void set_furthest_down(info_t *info, const int cur){
    long long nowsum = 0; 
    int idx = cur; 
    bool ready = true;//all children's info are set
    while(ready && idx != 0){
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
            info[u].mx = gen_dq();
            push_front(info[u].cnt - 1, info[u].presums[ info[u].cnt - 1 ], info[u].mx);
            for(int j = info[u].cnt - 2; j >= 0; j--){
                if(info[u].presums[j] > info[u].mx->front->val) push_front(j, info[u].presums[j], info[u].mx);
            }
            #ifdef debug
            print_dq(info[u].mx);
            #endif
            free(info[u].presums);
            info[u].presums = NULL;
            nowsum = info[u].mx->front->val;
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
vec_t *gen_vec(){
    vec_t *vec = malloc(sizeof(vec_t));
    vec->duns = malloc(sizeof(dun_t));
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
    vec->duns = realloc(vec->duns, vec->size * sizeof(dun_t));
    vec->duns[ vec->size - 1 ].id = id;
    vec->duns[ vec->size - 1 ].presum = vec->duns[ vec->size - 2 ].presum + len;
}
void pop_back(vec_t *vec){
    vec->size -= 1;
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
treasure_seq *gen_seq(long long val, int negpos, int cur){
    treasure_seq *seq = malloc(sizeof(treasure_seq));
    seq->top_id = seq->btm_id = cur;
    treasure_t *t = gen_t(val, negpos, NULL);
    seq->top = t, seq->btm = t;
    return seq;
}
int main(){
    int n, m, q;
    scanf("%d%d%d", &n, &m, &q);
    info_t *info = calloc(n, sizeof(info_t));
    int u, v; long long len;
    for(int i = 0; i < m; i++){
        scanf("%d%d%lld", &u, &v, &len);
        info[v].u = u, info[v].len = len, info[v].hasU = true;
        extend_down(info, u, v);
    }
    info[0].u = -1;
    for(int i = 0; i < n; i++){
        if(info[v].hasU && !info[i].cnt) set_furthest_down(info, i);
    }
    
    vec_t *vec = gen_vec();
    int cur = 0, op; long long val;
    for(int i = 0; i < q; i++){
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
                if(info[cur].seq != NULL){
                    if(info[cur].seq->top == info[cur].seq->btm){
                        #ifdef debug
                        printf("one treasure only, destroy directly\n");
                        #endif
                        destroy_seq(info[cur].seq);
                    }
                    else{
                        treasure_t *prev = info[cur].seq->btm->prev;
                        free(info[cur].seq->btm);
                        info[cur].seq->btm = prev;
                        info[cur].seq->btm->next = NULL;
                        info[cur].seq->btm_id = info[cur].u;
                        info[ info[cur].u ].seq = info[cur].seq;
                    }
                    info[cur].seq = NULL;
                }
                if(info[cur].mx != NULL){
                    destroy_dq(info[cur].mx);
                    info[cur].mx = NULL;
                }
                destroy_down(info, cur);
                if(info[ info[cur].u ].mx->front->i == info[cur].i) pop_front(info[ info[cur].u ].mx);
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
            else printf("%lld\n", info[cur].mx->front->val);
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
            #ifdef debug
            printf("ret = %d, id = %d, negpos = %d, val = %lld\n", ret, vec->duns[ret].id, negpos, val);
            #endif
            if(info[cur].seq == NULL){
                int cur_top = info[cur].u;
                if(info[cur_top].seq != NULL){
                    #ifdef debug
                    printf("%d's direct upstream(%d) has seq, extend\n", cur, cur_top);
                    #endif
                    info[cur].seq = info[cur_top].seq;
                    treasure_t *t = gen_t(val, negpos, info[cur].seq->btm);
                    info[cur].seq->btm->next = t, info[cur].seq->btm = t;
                    info[cur].seq->btm_id = cur;
                    info[cur_top].seq = NULL;
                }
                else info[cur].seq = gen_seq(val, negpos, cur);
            }
            else{
                treasure_t *t = gen_t(val, negpos, info[cur].seq->btm);
                info[cur].seq->btm->next = t, info[cur].seq->btm = t;
                info[cur].seq->btm_id = cur;
                int new_top_id = info[ info[cur].seq->top_id ].u;
                if(new_top_id == 0){
                    if(info[cur].seq->top->negpos != -1) printf("value lost at %d\n", info[cur].seq->top->negpos);
                    else printf("value remaining is %lld\n", info[cur].seq->top->val);
                    treasure_t *next = info[cur].seq->top->next;
                    free(info[cur].seq->top);
                    info[cur].seq->top = next, info[cur].seq->top->prev = NULL;
                }
                else if(info[ info[new_top_id].u ].seq == NULL) info[cur].seq->top_id = new_top_id;
                else{
                    info[ info[new_top_id].u ].seq->btm->next = info[cur].seq->top;
                    info[cur].seq->top->prev = info[ info[new_top_id].u ].seq->btm;
                    info[cur].seq->top_id = info[ info[new_top_id].u ].seq->top_id;
                    info[cur].seq->top = info[ info[new_top_id].u ].seq->top;
                    free(info[ info[new_top_id].u ].seq);
                    info[ info[new_top_id].u ].seq = NULL;
                }
            }
        }
        else break;
    }
    destroy_vec(vec);
    destroy_info(info, n);
    return 0;
}