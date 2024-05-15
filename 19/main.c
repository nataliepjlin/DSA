#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
typedef struct Node{
    long long sum;
    int val, prior, size;
    struct Node *left, *right;
}Node;
Node *init(const int val){
    Node *t = malloc(sizeof(Node));
    t->val = val, t->prior = rand(),  t->size = 1;
    t->sum = val;
    t->left = NULL, t->right = NULL;
    return t;
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
        *l = t;
        split(t->right, &(t->right), r, pos - getsize(t->left) - 1);//why modify pos here??
        update(t);
    }
    else{
        *r = t;
        split(t->left, l, &(t->left),pos);
        update(t);
    }
}
void print_treap(Node *t){
    if(!t) return;
    print_treap(t->left);
    printf("%d ", t->val);
    print_treap(t->right);
}
Node *insert(Node *t, int pos, const int val){
    Node *n = init(val), *x = NULL, *y = NULL;
    split(t, &x, &y, pos);
    return merge( merge(x, n), y);
}
Node* delete(Node *t, int pos){
    if(t == NULL || pos >= t->size) return t;
    Node *a, *b, *c, *d;
    split(t, &a, &b, pos - 1);
    split(b, &c, &d, 1);
    free(c);
    return merge(a, d);
}
void destroy_treap(Node *t){
    if(!t) return;
    destroy_treap(t->left);
    destroy_treap(t->right);
    free(t);
}
long long query(Node **t, int l, int r){
    Node *a = NULL, *b = NULL, *c = NULL;
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
        t = insert(t, i, val);
    }
    for(int i = 0; i < m; i++){
        scanf("%d", &op);
        if(op == 1){
            scanf("%d", &pos);
            t = delete(t, pos);
        }
        else if(op == 2){
            scanf("%d%d", &pos, &val);
            t = insert(t, pos, val);
        }
        else{
            scanf("%d%d", &l, &r);
            printf("%lld\n", query(&t, l, r));
        }
        #ifdef debug
        printf("op %d: ", i);
        print_treap(t);
        printf("\n");
        #endif
    }
    int s = getsize(t);
    if(s){
        printf("%d\n", s);
        print_treap(t);
        printf("\n");
    }
    else printf("0\n");
    destroy_treap(t);
    return 0;
}
