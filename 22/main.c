#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#define d 26
#define LEN 65
#define ui __uint64_t
typedef struct item{
    char s[LEN];
    ui hval;
    struct item *next;
}item;
typedef struct buc{
    int size;
    item *head, *tail;
}buc;
typedef struct big_buc{
    int q, r, pow2r, size;
    buc *buckets;
}big_buc;
ui rabinKarp(char *s){
    int len = strlen(s);
    ui t = 0;
    for(int i = 0; i < len; i++){
        t = (((d * t) & LLONG_MAX) + (s[i] - 'a')) & LLONG_MAX;
    }
    return t;
}
void destory_item(buc *buckets, const int r, const int q){
    int size = 1 << r + q;
    for(int i = 0; i < size; i++){
        #ifdef debug
        printf("%dth bucket, size = %d.", i, buckets[i].size);
        #endif
        if(buckets[i].head != NULL){
            item *cur = buckets[i].head;
            while(cur != NULL){
                #ifdef debug
                printf(" (%s, %lu)", cur->s, cur->hval);
                #endif
                item *nxt = cur->next;
                free(cur);
                cur = nxt;
            }
            buckets[i].head = NULL;
        }
        #ifdef debug
        printf("\n");
        #endif
    }
}
ui get_bit(const int r){
    return (r == 64) ? LLONG_MAX : ((ui)1 << r) - 1;
}
ui get_idx(item *it, const int r, const int q){
    ui idx = it->hval & get_bit(r);
    if(idx < q) idx = it->hval & get_bit(r + 1);
    return idx;
}
int push_back(buc *buckets, item *it, const int r, const int q){
    ui idx = get_idx(it, r, q);
    if(!buckets[idx].size){
        buckets[idx].head = buckets[idx].tail = it;
    }
    else{
        buckets[idx].tail->next = it;
        buckets[idx].tail = it;
    }
    buckets[idx].size++;
    return buckets[idx].size > 2;
}
void init_bb(big_buc *bb){
    bb->buckets = malloc(4 * sizeof(buc));
    for(int i = 0; i < 4; i++){
        bb->buckets[i].size = 0;
        bb->buckets[i].head = NULL;
    }
    bb->pow2r = 4, bb->q = 0, bb->r = 2, bb->size = 4;
}
void add_buc(big_buc *bb){
    bb->size += 1, bb->q += 1;
    int idx = bb->size - 1;
    bb->buckets = realloc(bb->buckets, bb->size * sizeof(buc));
    bb->buckets[idx].size = 0;
    bb->buckets[idx].head = bb->buckets[idx].tail = NULL;
    if(bb->pow2r == bb->q){
        bb->r += 1;
        bb->pow2r = (ui)1 << bb->r;
        bb->q = 0;
    }
    int cmpidx = idx & get_bit(bb->r);
    if(bb->buckets[cmpidx].size > 2){
        item *cur = bb->buckets[cmpidx].
    }
}
int main(){
    int n;
    big_buc bb;
    init_bb(&bb);
    scanf("%d", &n);
    for(int i = 0; i < n; i++){
        item *it = malloc(sizeof(item)); it->next = NULL;
        scanf("%s", it->s);
        it->hval = rabinKarp(it->s);
        if(push_back(bb.buckets, it, bb.r, bb.q)){//overflow

        }
    }
    destory_item(bb.buckets, bb.r, bb.q);
    free(bb.buckets);
}