#include <stdio.h>
#include <malloc.h>
#include <stdint.h>
#include <stdbool.h>
#define ui uint64_t
typedef struct company{
    int p, cnt;
    ui raise, total_r;//total_r += raise in every cnt
}company;
typedef struct employee{
    int cid;
    ui salary, balance;//balance = cnt * salary + total_r
}employee;
typedef struct comp_vec{
    company *c;
    int size;
}comp_vec;

int find(company *c, const int x, bool transing){
    if(c[x].p != x){
        int init_p = c[x].p;
        c[x].p = find(c, c[x].p, transing);
        if(c[init_p].p != init_p){//init_p not root
            c[x].cnt += c[init_p].cnt;
            c[x].total_r += (c[x].raise * c[init_p].cnt + c[init_p].total_r);
            c[x].raise += c[init_p].raise;
        }
    }
    else if(transing){
        c[x].cnt++;
        c[x].total_r += c[x].raise;
    }
    return c[x].p;
}
void quit(comp_vec *vec, employee *e, const int k){
    int cid = e[k].cid, root = -1;
    find(vec->c, cid, false);
    if(vec->c[cid].p != cid){//company not root
        root = vec->c[cid].p;
        e[k].balance += (vec->c[cid].total_r + vec->c[cid].raise * vec->c[root].cnt + 
        (vec->c[cid].cnt) * e[k].salary);
    }
    cid = vec->c[cid].p;
    e[k].balance += (vec->c[cid].total_r + vec->c[cid].cnt * e[k].salary);
}
void push_vec(comp_vec *vec){
    int idx = vec->size;
    vec->size += 1;
    vec->c = realloc(vec->c, vec->size * sizeof(company));
    vec->c[idx].p = idx, vec->c[idx].raise = 0, vec->c[idx].total_r = 0;
}
int main(){
    int n, q, op, k, x, y, p; ui s, r;
    scanf("%d%d", &n, &q);
    comp_vec vec; employee e[n];
    vec.c = malloc(sizeof(company) * n);
    vec.size = n;
    for(int i = 0; i < n; i++){
        scanf("%ld", &s);
        vec.c[i].p = i, vec.c[i].raise = 0, vec.c[i].total_r = 0;
        e[i].cid = i, e[i].balance = 0, e[i].salary = s;
    }
    for(int i = 0; i < q; i++){
        scanf("%d", &op);
        if(op == 1){//merge
            scanf("%d%d", &x, &y); x--, y--;
            int v = find(vec.c, e[x].cid, false), w = find(vec.c, e[y].cid, false);
            if(v == w) continue;
            push_vec(&vec);
            vec.c[v].p = vec.c[w].p = vec.size - 1;
            #ifdef debug
            printf("merge company %d, %d to %d, that employ %d and %d\n", v, w, vec.size - 1, x, y);
            #endif
        }
        else if(op == 2){//raise
            scanf("%d%ld", &k, &r); k--;
            p = find(vec.c, e[k].cid, false);
            vec.c[p].raise += r;
            #ifdef debug
            printf("%d is in company %d, raise = %ld\n", k, p, vec.c[p].raise);
            #endif
        }
        else if(op == 3){//transfer
            scanf("%d", &k); k--;
            p = find(vec.c, e[k].cid, true);
            #ifdef debug
            printf("%d is in company %d, transfer\n", k, p);
            #endif
        }
        else{//quit
            scanf("%d%ld", &k, &s); k--;
            quit(&vec, e, k);
            push_vec(&vec);
            e[k].cid = vec.size - 1, e[k].salary = s;
        }
    }
    for(int i = 0; i < n; i++){
        quit(&vec, e, i);
        printf("%ld%c", e[i].balance, " \n"[i == n - 1]);
    }
    free(vec.c);
}