#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
typedef struct Node{
    long long preSum;
    struct Node *next;
}Node;
typedef struct Info{
    Node *head;
    Node *left, *right;
    int cnt;//attack
}Info;
typedef struct BigInfo{
    Info **info;
}BigInfo;
Node *genNode(long long data){
    Node *node = (Node*)malloc(sizeof(Node));
    node->preSum = data, node->next = NULL;
    return node;
}
void initInfo(BigInfo *list, const int n){
    list->info = (Info**)malloc(sizeof(Info*) * n);
    for(int i = 0; i < n; i++){
        list->info[i] = (Info*)malloc(sizeof(Info));
        list->info[i]->head = genNode(0ll);
        list->info[i]->left = list->info[i]->head;
        list->info[i]->right = list->info[i]->head;
        list->info[i]->cnt = 0;
    }
}
void extendInfo(Info *info, long long powDiff, const int m){
    Node *newNode = genNode(info->right->preSum + powDiff);
    info->right->next = newNode;
    info->right = info->right->next;
    if(info->cnt >= m) info->left = info->left->next;
    info->cnt += 1;
}
int min(int a, int b){
    return a < b ? a : b;
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
            if(b == m || b >= list.info[a]->cnt){
                printf("%lld\n", list.info[a]->right->preSum - list.info[a]->left->preSum);
            }
            else{
                #ifdef debug
                printf("b = %d, m = %d, list.info[a]->cnt = %d\n", b, m, list.info[a]->cnt);
                #endif
                int step = min(m, list.info[a]->cnt) - b, fastStep = step >> 1;
                Node *tmpLeft = list.info[a]->left;
                for(int i = 0; i < fastStep; i++)
                    tmpLeft = tmpLeft->next->next;
                if(step & 1) tmpLeft = tmpLeft->next;
                printf("%lld\n", list.info[a]->right->preSum - tmpLeft->preSum);
            }
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
        while(list.info[i]->head->next){
            printf(" %lld", list.info[i]->head->next->preSum - list.info[i]->head->preSum);
            list.info[i]->head = list.info[i]->head->next;
        }
        printf("\n");
    }
}