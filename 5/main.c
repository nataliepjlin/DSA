#include <stdio.h>
#include <stdlib.h>
#define MAXM 5 * 1e5 + 5
typedef struct Node{
    long long preSum;
}Node;
typedef struct Info{
    Node **head;
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
void initInfo(BigInfo *list, const int n){
    list->info = (Info**)malloc(sizeof(Info*) * n);
    for(int i = 0; i < n; i++){
        list->info[i] = (Info*)malloc(sizeof(Info));
        list->info[i]->head = (Node**)malloc(sizeof(Node*) * MAXM);
        list->info[i]->head[0] = genNode(0ll);
        #ifdef debug
        printf("list->info[%d]->head[0] = %lld\n", i, list->info[i]->head[0]->preSum);
        #endif
        list->info[i]->cnt = 0;
    }
}
void extendInfo(Info *info, long long powDiff, const int m){
    Node *newNode = genNode(info->head[info->cnt]->preSum + powDiff);
    info->cnt += 1;
    info->head[info->cnt] = newNode;
    #ifdef debug
    printf("info->head[%d] = %lld\n", info->cnt, info->head[info->cnt]->preSum);
    #endif
}
int main(){
    int n, t, m;
    scanf("%d%d%d", &n, &t, &m);
    BigInfo list;//info with player's id
    initInfo(&list, n);
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
            if(b > idx) printf("%lld\n", list.info[a]->head[idx]->preSum);
            else printf("%lld\n", list.info[a]->head[idx]->preSum - list.info[a]->head[idx - b]->preSum);
        }
        else if(op == 1){
            scanf("%d", &a);
            a--;
            int rankA = rank[a];
            if(rankA == 0){
                #ifdef debug
                printf("rank of %d is already at top\n", a);
                #endif
            }
            else{
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
        if(!list.info[i]->cnt){
            printf("0\n");
            continue;
        }
        printf("%d", list.info[i]->cnt);
        for(int j = 1; j <= list.info[i]->cnt; j++){
            printf(" %lld", list.info[i]->head[j]->preSum - list.info[i]->head[j - 1]->preSum);
        }
        printf("\n");
    }
}