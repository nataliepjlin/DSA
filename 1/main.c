#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#define N 25
#define MX 200005
long long ans[MX][N];
int cmp(const void *a, const void *b){
    long long *pa = (long long*)a, *pb = (long long*)b;
    if(*pa > *pb) return 1;
    else if(*pa < *pb) return -1;
    return 0;
}
void f(int idx, int n, bool used[], long long arr[], long long perm[], int *cnt){
    if(idx >= 3
    && !((perm[idx - 2] - perm[idx - 3] > 0 && perm[idx - 1] - perm[idx - 2] < 0)
    || (perm[idx - 2] - perm[idx - 3] < 0 && perm[idx - 1] - perm[idx - 2] > 0)))
        return;
    if(idx == n){
        for(int i = 0; i < n; i++)
            ans[*cnt][i] = perm[i];
        *cnt += 1;
        return;
    }
    for(int i = 0; i < n; i++){
        if((i && arr[i] == arr[i - 1] && !used[i - 1])
        || used[i]) continue;
        used[i] = true;
        perm[idx] = arr[i];
        f(idx + 1, n, used, arr, perm, cnt);
        perm[idx] = 0;
        used[i] = false;
    }
}
int main(){
    int n;
    scanf("%d", &n);
    long long arr[N], perm[N];
    bool used[N] = {0};
    for(int i = 0; i < n; i++){
        scanf("%lld", &arr[i]);
    }
    qsort(arr, n, sizeof(long long), cmp);
    int cnt = 0;
    f(0, n, used, arr, perm, &cnt);
    printf("%d\n", cnt);
    for(int i = 0; i < cnt; i++){
        for(int j = 0; j < n; j++)
            printf("%lld%c", ans[i][j], " \n"[j == n - 1]);
    }
    return 0;
    
}