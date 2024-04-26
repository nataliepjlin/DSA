#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#define LEN 5000001
char text[LEN];//s
char pattern[LEN];//c
typedef struct HitNode{
    bool change;
    int cnt;
}HitNode;
typedef struct Hits{
    int size;
    HitNode *nodes;
}HitVec;
void initVec(const HitVec *prev, const HitVec *ref, HitVec *vec, const int size){
    vec->size = size;
    vec->nodes = malloc(size * sizeof(HitNode));
    #ifdef debug
    printf("\nvec #%d\n", size - 1);
    #endif
    for(int i = 0; i < size - 1; i++){
        vec->nodes[i].cnt = prev->nodes[i].cnt;
        vec->nodes[i].change = false;
        if(ref && i < ref->size && ref->nodes[i].change){
            vec->nodes[i].cnt += 1;
            vec->nodes[i].change = true;
        }
        #ifdef debug
        printf("%dth node: (%d, %s)\n", i, vec->nodes[i].cnt, (vec->nodes[i].change) ? "true" : "false");
        #endif
    }
    vec->nodes[size - 1].cnt = 1;
    vec->nodes[size - 1].change = true;
}
void destroyVec(HitVec *vecs, const int m){
    for(int i = 0; i < m; i++){
        free(vecs[i].nodes);
        vecs[i].nodes = NULL;
        vecs[i].size = 0;
    }
}
int createLPS(const int len, long long *lps, long long *plag, HitVec *vecs){
    lps[0] = 0, plag[0] = 1;
    int prev = 0, cur = 1, cnt = 0;
    while(cur < len){
        if(pattern[prev] == pattern[cur]){
            lps[cur] = prev + 1;
            plag[cur] = plag[ lps[cur] - 1 ] + 1;
            initVec(&(vecs[cur - 1]), &(vecs[lps[cur] - 1]), &(vecs[cur]), cur + 1);
            prev++, cur++;
        }
        else{
            if(prev != 0) prev = lps[prev - 1];
            else{
                lps[cur] = 0;
                plag[cur] = 1;
                initVec(&(vecs[cur - 1]), NULL, &(vecs[cur]), cur + 1);
                cur++;
            }
        }
    }
}
int main(){
    scanf("%s%s", text, pattern);
    const int n = strlen(text), m = strlen(pattern);
    long long lps[m], plag[m];
    HitVec vecs[m];
    vecs[0].size = 1;
    vecs[0].nodes = malloc(sizeof(HitNode));
    vecs[0].nodes[0].change = true, vecs[0].nodes[0].cnt = 1;
    createLPS(m, lps, plag, vecs);
    int idx[n], cnt = 0;
    destroyVec(vecs, m);
    return 0;
}