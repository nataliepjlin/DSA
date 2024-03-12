#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
typedef struct Node{
    long long preSum;
}Node;
typedef struct Info{
    Node **arr;
    Node *cur;
    int cnt;//attack
}Info;
typedef struct BigInfo{
    Info **info;
}BigInfo;
Node *genNode(long long data){
    Node *node = (Node*)malloc(sizeof(Node));
    node->preSum = data;
    return node;
}
void initInfo(BigInfo *list, const int n, const int t){
    list->info = (Info**)malloc(sizeof(Info*) * n);
    for(int i = 0; i < n; i++){
        list->info[i] = (Info*)malloc(sizeof(Info));
        list->info[i]->arr = (Node**)malloc(sizeof(Node*) * (t + 1));
        list->info[i]->arr[0] = genNode(0ll);
        list->info[i]->cnt = 0;
    }
}
void extendInfo(Info *info, long long powDiff, const int m){
    info->cnt += 1;
    info->arr[info->cnt] = genNode(info->arr[info->cnt - 1]->preSum + powDiff);
}
int main(){
    int n, t, m;
    scanf("%d%d%d", &n, &t, &m);
    BigInfo list;//info with player's id
    initInfo(&list, n, t);
    long long powers[n];//power of ith ranking player
    int rank[n];//use id to find rank
    int ids[n];//use rank to find id
    for(int i = 0; i < n; i++){
        scanf("%lld", &powers[i]);
        rank[i] = i, ids[i] = i;
    }
    int op, a, b;
    long long op2 = 0, q;
    for(int i = 0; i < t; i++){
        scanf("%d", &op);
        if(op == 4){
            scanf("%d%d", &a, &b);
            a--;
            int idx = list.info[a]->cnt;
            if(idx <= b) printf("%lld\n", list.info[a]->arr[idx]->preSum);
            else printf("%lld\n", list.info[a]->arr[idx]->preSum - list.info[a]->arr[idx - b]->preSum);
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
                extendInfo(list.info[a], powDiff, m);
                #ifdef debug
                printf("player %d gain pow %lld\n", a, powDiff);
                #endif
                rank[a]--, rank[prevID]++;
                ids[rankA - 1] = a, ids[rankA] = prevID;
            }
        }
        else if(op == 2) op2++;
        else{//op == 3
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
        #ifdef debug
        printf("\n%dth task:\n", i);
        for(int j = 0; j < n; j++){
            printf("id: %d, rank: %d, pow = %lld\n", j, rank[j], powers[rank[j]] + op2 * (n - 1 - rank[j]));
        }
        printf("-----\n");
        #endif
    }
    printf("\n");
    for(int i = 0; i < n; i++){//second part
        printf("%d", list.info[i]->cnt);
        for(int j = 1; j <= list.info[i]->cnt; j++)
            printf(" %lld", list.info[i]->arr[j]->preSum - list.info[i]->arr[j - 1]->preSum);
        printf("\n");
        free(list.info[i]->arr);
    }
    free(list.info);
}