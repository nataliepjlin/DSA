#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
typedef struct vector_t{
    long long *presums;
    int cap;
    int size;
}vector_t;
vector_t *vector_new(){
    return (vector_t*)malloc(sizeof(vector_t));
}
vector_t **bigVec_new(const int n){
    return (vector_t**)malloc(sizeof(vector_t*) * n);
}
void vector_init(vector_t *obj){
    obj->presums = calloc(2, sizeof(long long));
    obj->cap = 1;
    obj->size = 0;
}
void vector_destroy(vector_t *obj){
    free(obj->presums);
    free(obj);
}
void push_back(vector_t *obj, const long long sum){
    #ifdef debug
    printf("sum = %lld\n", sum);
    #endif
    if(obj->size != 0){
        obj->cap += 1;//cap = 1
        obj->presums = realloc(obj->presums, (obj->cap + 1) * sizeof(long long));//real cap = 2(cap + 1)
        #ifdef debug
        printf("obj->size = %d, realloc %d\n", obj->size, obj->cap);
        #endif
    }
    assert(obj->presums != NULL);
    obj->presums[obj->size + 1] = sum + obj->presums[obj->size];
    obj->size += 1;
}
int main(){
    int n, t, m;
    scanf("%d%d%d", &n, &t, &m);
    vector_t **bigVec = bigVec_new(n);
    long long powers[n];//power of ith ranking player
    int rank[n];//use id to find rank
    int ids[n];//use rank to find id
    for(int i = 0; i < n; i++){
        scanf("%lld", &powers[i]);
        rank[i] = i, ids[i] = i;
        bigVec[i] = vector_new();
        vector_init(bigVec[i]);
    }
    int op, a, b;
    long long op2 = 0, q;
    for(int i = 0; i < t; i++){
        scanf("%d", &op);
        if(op == 2) op2++;
        else if(op == 3){
            scanf("%lld", &q);
            if(q > powers[0] + op2 * (n - 1)){
                printf("0 0\n");
                continue;
            }
            int low = 0, high = n - 1, rnk = -1;
            while(low <= high){
                int mid = (low + high) >> 1;
                if(powers[mid] + op2 * (n - 1 - mid) >= q){
                    rnk = mid;
                    low = mid + 1;
                }
                else high = mid - 1;
            }
            printf("%d %d\n", rnk + 1, ids[rnk] + 1);
        }
        else if(op == 1){
            scanf("%d", &a);
            a--;
            int rankA = rank[a];
            if(rankA != 0){
                int prevID = ids[rankA - 1];
                #ifdef debug
                printf("swap player %d and %d's ranking\n", prevID, a);
                #endif
                long long powDiff = (powers[rankA - 1] - powers[rankA]) + op2;
                powers[rankA] += powDiff;
                push_back(bigVec[a], powDiff);
                #ifdef debug
                printf("player %d gain pow %lld\n", a, powDiff);
                #endif
                rank[a]--, rank[prevID]++;
                ids[rankA - 1] = a, ids[rankA] = prevID;
            }
        }
        else{//op = 4;
            scanf("%d%d", &a, &b);
            a--;
            int idx = bigVec[a]->size;
            if(b >= idx) printf("%lld\n", bigVec[a]->presums[idx]);
            else printf("%lld\n", bigVec[a]->presums[idx] - bigVec[a]->presums[idx - b]);
        }
        #ifdef debug
        printf("\n%dth task:\n", i);
        for(int j = 0; j < n; j++){
            printf("id: %d, rank: %d, pow = %lld\n", j, rank[j], powers[rank[j]] + op2 * (n - 1 - rank[j]));
        }
        printf("-----\n");
        #endif
    }
    printf("\n");
    for(int i = 0; i < n; i++){
        printf("%d", bigVec[i]->size);
        for(int j = 1; j <= bigVec[i]->size; j++)
            printf(" %lld", bigVec[i]->presums[j] - bigVec[i]->presums[j - 1]);
        printf("\n");
        vector_destroy(bigVec[i]);
    }
    free(bigVec);
    return 0;
}   