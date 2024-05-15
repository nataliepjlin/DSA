#include <stdio.h>
#include <stdlib.h>
#include <time.h>
typedef struct Node{
    long long sum;
    int val, prior, size;
    int idx;//for debugging
    struct Node *left, *right;
}Node;
typedef struct Pair{
    Node *first, *second;
}Pair;
Node *init(const int val, const int idx){
    Node *t = malloc(sizeof(Node));
    t->val = val, t->prior = rand(),  t->size = 1;
    t->sum = val;
    t->idx = idx;
    t->left = NULL, t->right = NULL;
}
int getsize(Node *t){
    return t ? t->size : 0;
}
void update(Node *t){
    t->size = 1, t->sum = t->val;
    if(t->left){
        t->size += t->left->size;
        t->sum += t->left->sum;
    }
    if(t->right){
        t->size += t->right->size;
        t->sum += t->right->sum;
    }
}
Node *merge(Node *a, Node *b){
    if(!a) return b;
    if(!b) return a;
    if(a->prior > b->prior){
        a->right = merge(a->right, b);
        update(a);
        return a;
    }
    else{
        b->left = merge(a, b->left);
        update(b);
        return b;
    }
}
void split(Node *t, Node **l, Node **r, const int pos){
    if(!t){
        *l = *r = NULL;
        return;
    }
    if(getsize(t->left) < pos){
        split(t->right, &(t->right), r, pos - getsize(t->left) - 1);//why modify pos here??
        update(t);
        *l = t;
    }
    else{
        split(t->left, l, &(t->left),pos);
        update(t);
        *r = t;
    }
}
void print_treap(Node *t){
    if(!t) return;
    print_treap(t->left);
    printf("%d ", t->val);
    print_treap(t->right);
}
void insert(Node **t, int pos, const int val){
    Node *a = init(val, pos), *b;
    split(*t, t, &b, pos);
    *t = merge( merge(*t, a), b);
}
void delete(Node **t, int pos){
    if(pos >= (*t)->size) return;
    Node *a, *b, *c, *d;
    split(*t, &a, &b, pos - 1);
    split(b, &c, &d, 1);
    *t = merge(a, d);
}
void destroy_treap(Node *t){
    if(!t) return;
    destroy_treap(t->left);
    destroy_treap(t->right);
    free(t);
}
long long query(Node **t, int l, int r){
    Node *a = NULL, *b = NULL;
    split(*t, t, &a, r);
    split(*t, t, &b, l - 1);
    long long q = (b) ? b->sum : 0;
    *t = merge(*t, b);
    *t = merge(*t, a);
    return q;
}
int main(){
    int n, m, val, op, pos, l, r;
    Node *t = NULL;
    scanf("%d%d", &n, &m);
    for(int i = 0; i < n; i++){
        scanf("%d", &val);
        insert(&t, i, val);
        #ifdef debug
        print_treap(t);
        printf("\n");
        #endif
    }
    for(int i = 0; i < m; i++){
        scanf("%d", &op);
        if(op == 1){
            scanf("%d", &pos);
            delete(&t, pos);//if t is null?
        }
        else if(op == 2){
            scanf("%d%d", &pos, &val);
            insert(&t, pos, val);
        }
        else{
            scanf("%d%d", &l, &r);
            printf("%lld\n", query(&t, l, r));
        }
    }
    int s;
    if(s = getsize(t)){
        printf("%d\n", s);
        print_treap(t);
        printf("\n");
    }
    else printf("0\n");
    destroy_treap(t);
    return 0;
}

