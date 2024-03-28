#include <stdio.h>
#include <stdlib.h>
#define N 1000001
enum OP{DOWN = 1, UP, PLAN, GUESS, DISCOVER};
typedef struct path_t path_t;
typedef struct dungent_t dungent_t;
struct path_t{
    int len, u, v;
    path_t *prev_down, *next_down;
};
struct dungent_t{
    long long treasure;
    path_t *down_h, *down_cur, *up;
};
dungent_t *init_dungent(){
    dungent_t *d = malloc(sizeof(dungent_t));
    d->treasure = -1;
    d->down_h = d->down_cur = NULL;
    d->up = NULL;
    return d;
}
path_t *init_path(int u, int v, int len){
    path_t *p = malloc(sizeof(path_t));
    p->len = len, p->u = u, p->v = v;
    p->prev_down = NULL;
    p->next_down = NULL;
    return p;
}
void print_path(path_t *p){
    while(p){
        printf("(%d, %d, %d)->", p->u, p->v, p->len);
        p = p->next_down;
    }
    printf("X\n");
}
int main(){
    int n, m, q, u, v, len, cur = 0, op;
    long long ti;
    scanf("%d%d%d", &n, &m, &q);
    dungent_t *dunArr[n];
    for(int i = 0; i < n; i++) dunArr[i] = init_dungent();
    for(int i = 0; i < m; i++){//create vector here???
        scanf("%d%d%d", &u, &v, &len);
        path_t *p = init_path(u, v, len);
        if(dunArr[u]->down_h == NULL){
            dunArr[u]->down_h = p;
            dunArr[u]->down_cur = dunArr[u]->down_h;
        }
        else{
            dunArr[u]->down_cur->next_down = p;
            p->prev_down = dunArr[u]->down_cur;
            dunArr[u]->down_cur = dunArr[u]->down_cur->next_down;
        }
        dunArr[v]->up = p;//use same path node, may cause RE???
        #ifdef debug
        printf("down_h: ");
        print_path(dunArr[u]->down_h);
        printf("\nup: ");
        print_path(dunArr[v]->up);
        printf("\n");
        #endif
    }
    for(int i = 0; i < q; i++){
        scanf("%d", &op);
        #ifdef debug
        printf("op = %d\n", op);
        #endif
        if(op == DOWN){
            if(dunArr[cur]->down_h == NULL) printf("-1\n");
            else{
                cur = dunArr[cur]->down_h->v;
                printf("%d\n", cur);
            }
        }
        else if(op == UP){
            if(cur == 0) printf("-1\n");
            else{
                v = cur, u =  dunArr[v]->up->u;
                #ifdef debug
                printf("u = %d, v = %d\n", u, v);
                #endif
                path_t *p = dunArr[v]->up;
                dunArr[v]->up = NULL;
                if(p->prev_down == NULL){//p == dunArr[u]->down_h
                    dunArr[u]->down_h = (dunArr[u]->down_h)->next_down;
                    if(dunArr[u]->down_h)
                        dunArr[u]->down_h->prev_down = NULL;
                }
                else{
                    (p->prev_down)->next_down = p->next_down;
                    if(p->next_down) (p->next_down)->prev_down = p->prev_down;
                }
                free(p);
                cur = u;
                printf("%d\n", cur);
                #ifdef debug
                printf("down_h: ");
                print_path(dunArr[u]->down_h);
                printf("\nup: ");
                print_path(dunArr[v]->up);
                printf("\n");
                #endif
            }
        }
        else if(op == PLAN){
            scanf("%lld", &ti);
            int tmp_cur = cur;
            long long newti;
            while(tmp_cur 
            && (newti = ti - dunArr[tmp_cur]->up->len) >= 0){
                tmp_cur = dunArr[tmp_cur]->up->u;
                ti = newti;
            }//change to binary search+prefix sum????
            printf("%d\n", tmp_cur);
        }
        else break;
    }
}