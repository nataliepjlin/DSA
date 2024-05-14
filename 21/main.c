#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
enum COLOR{B, R};
typedef struct Node{
    int color, key;
    struct Node *p, *left, *right;
}Node;
typedef struct Tree{
    Node *root, *nil;
}Tree;
void print_tree(Tree *t, Node *cur){
    if(cur == t->nil) return;
    printf("%d ", cur->key);
    print_tree(t, cur->left);
    print_tree(t, cur->right);
}
void left_rot(Tree *t, Node *x){
    Node *y = x->right;
    x->right = y->left;
    if(y->left != t->nil) y->left->p = x;
    y->p = x->p;
    assert(x->p != NULL);
    if(x->p == t->nil) t->root = y;
    else if(x == x->p->left) x->p->left = y;
    else x->p->right = y;
    y->left = x;
    x->p = y;
}
void right_rot(Tree *t, Node *y){
    Node *x = y->left;
    y->left = x->right;
    if(x->right != t->nil) x->right->p = y;
    x->p = y->p;
    if(y->p == t->nil) t->root = x;
    else if(y == y->p->left) y->p->left = x;
    else y->p->right = x;
    x->right = y;
    y->p = x;
}
Node *gen_node(const int color, const int key, Node *p, Node *nil){
    Node *node = malloc(sizeof(Node));
    node->color = color, node->key = key;
    node->p = p;
    node->left = node->right = nil;
    return node;
}
Tree *gen_tree(){
    Tree *t = malloc(sizeof(Tree));
    t->nil = gen_node(B, -1, NULL, NULL);
    assert(t->nil != NULL);
    t->root = t->nil;
    return t;
}
void insert(Tree *t, int color, const int key){
    if(t->root == t->nil){
        Node *node = gen_node(color, key, t->nil, t->nil);
        t->root = node;
        return;
    }
    Node *cur = t->root;
    while(cur != t->nil){
        if(key < cur->key){
            if(cur->left != t->nil) cur = cur->left;
            else{
                cur->left = gen_node(color, key, cur, t->nil);
                return;
            }
        }
        else{
            if(cur->right != t->nil) cur = cur->right;
            else{
                cur->right = gen_node(color, key, cur, t->nil);
                return;
            }
        }
    }
}
void destroy_node(Node *node){
    if(node->left->key != -1) destroy_node(node->left);
    if(node->right->key != -1) destroy_node(node->right);
    free(node);
}
void destroy_tree(Tree *t){
    if(t->root != t->nil) destroy_node(t->root);
    free(t->nil);
    free(t);
}
Node *find_node(Tree *t, const int key, int *d, int *bd){
    *d = 0, *bd = 0;
    Node *cur = t->root;
    while(cur != t->nil){
        if(key == cur->key) return cur;
        else{
            *d += 1, *bd += (cur->color == B);
            if(key < cur->key) cur = cur->left;
            else cur = cur->right;
        }
    }
    return NULL;
}
int main(){
    int n, m, key, d, bd;
    char input[2];
    scanf("%d", &n);
    Tree *t = gen_tree();
    for(int i = 0; i < n; i++){
        scanf("%s%d", input, &key);
        insert(t, input[0] == 'R', key);
    }

    scanf("%d", &m);
    for(int i = 0; i < m; i++){
        scanf("%s%d", input, &key);
        #ifdef debug
        printf("input = %s, key = %d\n", input, key);
        #endif
        if(input[0] == 'L'){
            Node *node = find_node(t, key, &d, &bd);
            left_rot(t, node);
        }
        else if(input[0] == 'R'){
            Node *node = find_node(t, key, &d, &bd);
            right_rot(t, node);
        }
        else{
            Node *node = find_node(t, key, &d, &bd);
            printf("%d %d %d %d\n", node->left->key, node->right->key, bd, d);
        }
    }
    destroy_tree(t);
    return 0;
}